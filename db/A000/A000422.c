/*
 * A000422  Concatenation of numbers from n down to 1.
 * https://oeis.org/A000422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*10^len(a(n-1)) + a(n-1), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/num_digits_idx.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t power = 10;
    
    for (idx_t k = 2; k <= n; k++) {
        result = (seq_t)k * power + result;
        
        int k_digits = num_digits_idx(k);
        for (int i = 0; i < k_digits; i++) {
            power *= 10;
        }
    }
    
    return result;
}
