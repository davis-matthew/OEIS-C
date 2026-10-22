/*
 * A001704  a(n) = n concatenated with n + 1.
 * https://oeis.org/A001704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n concatenated with n + 1
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 9677, u64 n <= 9677,
 *            u128 n <= 9677, big any n
 * Helpers:   pow10_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_helper.h"

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n_plus_1 = nseq + 1;
    
    // Count digits in n+1
    int digits = 0;
    seq_t temp = n_plus_1;
    while (temp > 0) {
        digits++;
        temp /= 10;
    }
    
    // Concatenate: n * 10^digits + (n+1)
    return nseq * pow10_helper(digits) + n_plus_1;
}
