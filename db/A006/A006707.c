/*
 * A006707  Expansion of a modular function.
 * https://oeis.org/A006707
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy
 * Formula:   eta(2z)^11*eta(3z)^13/(eta(z)^7*eta(6z)^17)
 * Range:     i8 n <= 4, i16 n <= 19, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *coeff = NULL;
static idx_t coeff_len = 0;
static idx_t computed_up_to = -3;

seq_t a(idx_t n) {
    if (n < -2) return 0;
    
    idx_t index = n + 2;
    
    if (index <= computed_up_to) {
        return coeff[index];
    }
    
    idx_t needed_len = index + 1;
    if (needed_len > coeff_len) {
        idx_t new_len = needed_len + 100;
        seq_t *new_coeff = (seq_t *)realloc(coeff, (size_t)new_len * sizeof(seq_t));
        if (!new_coeff) return 0;
        
        for (idx_t i = coeff_len; i < new_len; i++) {
            new_coeff[i] = 0;
        }
        
        coeff = new_coeff;
        coeff_len = new_len;
    }
    
    if (computed_up_to < 0) {
        coeff[0] = 1;
        computed_up_to = 0;
    }
    
    for (idx_t k = computed_up_to + 1; k <= index; k++) {
        int exp_k = -7;
        if (k % 2 == 0) exp_k += 11;
        if (k % 3 == 0) exp_k += 13;
        if (k % 6 == 0) exp_k -= 17;
        
        if (exp_k > 0) {
            for (int m = 0; m < exp_k; m++) {
                for (idx_t i = coeff_len - 1; i >= k; i--) {
                    coeff[i] -= coeff[i - k];
                }
            }
        } else if (exp_k < 0) {
            for (int m = 0; m < -exp_k; m++) {
                for (idx_t i = k; i < coeff_len; i++) {
                    coeff[i] += coeff[i - k];
                }
            }
        }
    }
    
    computed_up_to = index;
    return coeff[index];
}
