/*
 * A004083  Numbers k such that cos(k-1) <= 0 and cos(k) > 0.
 * https://oeis.org/A004083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that cos(k-1) <= 0 and cos(k) > 0; search
 *            algorithm
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    
    while (count < n) {
        k++;
        double cos_k_minus_1 = cos((double)(k - 1));
        double cos_k = cos((double)k);
        
        if (cos_k_minus_1 <= 0 && cos_k > 0) {
            count++;
        }
    }
    
    return (seq_t)k;
}
