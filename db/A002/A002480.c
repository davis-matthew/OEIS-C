/*
 * A002480  Numbers of the form 2x^2 + 3y^2.
 * https://oeis.org/A002480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number of the form 2x^2 + 3y^2
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t limit = 10 * n;
    if (limit < 100) limit = 100;
    
    while (1) {
        unsigned char *is_rep = (unsigned char *)malloc(limit + 1);
        if (!is_rep) return (seq_t)0;
        
        for (idx_t i = 0; i <= limit; i++) {
            is_rep[i] = 0;
        }
        
        for (idx_t x = 0; ; x++) {
            idx_t two_x_sq = 2 * x * x;
            if (two_x_sq > limit) break;
            
            for (idx_t y = 0; ; y++) {
                idx_t val = two_x_sq + 3 * y * y;
                if (val > limit) break;
                is_rep[val] = 1;
            }
        }
        
        idx_t count = 0;
        for (idx_t i = 0; i <= limit; i++) {
            if (is_rep[i]) {
                count++;
                if (count == n) {
                    seq_t result = (seq_t)i;
                    free(is_rep);
                    return result;
                }
            }
        }
        
        free(is_rep);
        limit *= 2;
    }
}
