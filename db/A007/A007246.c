/*
 * A007246  McKay-Thompson series of class 2B for the Monster group.
 * https://oeis.org/A007246
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   Expansion of 24 + chi(-q)^24 / q where chi(-q) = prod_{k>=0} (1
 *            - q^(2k+1))
 * Range:     i8 n <= 0, i16 n <= 3, i32 n <= 14, i64 n <= 56, i128 n <= 217,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 961 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    if (n < -1) return 0;
    
    idx_t degree = n + 1;
    
    seq_t *chi = mem_new(seq_t, degree + 1);
    chi[0] = 1;
    
    for (idx_t k = 0; (2 * k + 1) <= degree; k++) {
        idx_t exp = 2 * k + 1;
        for (idx_t i = degree; i >= exp; i--) {
            chi[i] -= chi[i - exp];
        }
    }
    
    seq_t *result = mem_new(seq_t, degree + 1);
    result[0] = 1;
    
    seq_t *base = mem_new(seq_t, (degree + 1));
    for (idx_t i = 0; i <= degree; i++) {
        base[i] = chi[i];
    }
    
    for (int pow = 24; pow > 0; pow >>= 1) {
        if (pow & 1) {
            seq_t *temp = mem_new(seq_t, degree + 1);
            for (idx_t i = 0; i <= degree; i++) {
                for (idx_t j = 0; i + j <= degree; j++) {
                    temp[i + j] += result[i] * base[j];
                }
            }
            mem_free(result);
            result = temp;
        }
        
        if (pow > 1) {
            seq_t *temp = mem_new(seq_t, degree + 1);
            for (idx_t i = 0; i <= degree; i++) {
                for (idx_t j = 0; i + j <= degree; j++) {
                    temp[i + j] += base[i] * base[j];
                }
            }
            mem_free(base);
            base = temp;
        }
    }
    
    seq_t coeff;
    if (n == -1) {
        coeff = result[0];
    } else if (n == 0) {
        coeff = 24 + result[1];
    } else {
        coeff = result[n + 1];
    }
    
    mem_free(chi);
    mem_free(result);
    mem_free(base);
    
    return coeff;
}
