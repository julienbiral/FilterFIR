#ifndef FILTERFIR_H
#define FILTERFIR_H

#include <cstdint>

template<int N>
class FilterFIR {

private:
	int16_t values[N];	
	int16_t coef[N];
	float gain;	
	int k;

public:
	FilterFIR(){
		k = 0;
		for (int i=0; i<N; i++) {      
			values[i] = 0;
			coef[i] = 0;
		}
		// Default coef (allpass filter)
		coef[0] = 1;
		gain = 1.0;
	}
	
	FilterFIR(float *newCoefs) {
		k = 0;
		for (int i=0; i<N; i++) {      
			values[i] = 0;
		}
		setCoefficients(newCoefs);
	}

	void setGain(float newGain) {
		gain = newGain;
	}
	
	void setCoefficients(const int16_t *newCoefs) {
		for (int i=0; i<N; i++) {      
			coef[i] = newCoefs[i];
		}           
	}
	
	int16_t process(int16_t input) {
		// Input and coefficients are coded on 16 bits. To avoid overflow in the multiplication process, the output must be coded on 32 bits.
		int32_t output = 0;
                
		// Store the input of the routine (contents of the 'in' variable) in the array at the current pointer position
		values[k] = input;

		for (int i=0; i<N; i++) {
			output += (int32_t) coef[i] * (int32_t) values[(i + k) & (N - 1)];
			// If N is not a power of two, uncomment next line
			// output += (int32_t) coef[i] * (int32_t) values[(i + k) % N];
		}
                
		// Shift by 15 bits to the right (16 bits signed) to rescale the output
		output = output >> 15;
		// More precise gain adjustment
		output = (int16_t) (gain * (float) output);
                
		// Compute the modulo of a power of two to increase the speed
		// If N is not a power of two, uncomment next line
		// k = (k+1) % N ;
		k = (k+1) & (N - 1) ;

		return ((int16_t) output);
	}
};

#endif
