/*
 * A169722  a(n) = (3*2^(n-1)-1)*(18*2^(n-1)-7).
 * https://oeis.org/A169722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^(n-1)-1)*(18*2^(n-1)-7), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* 2^(n-1) */
    seq_t p = pow2(n - 1);
    seq_t term1 = 3 * p - 1;
    seq_t term2 = 18 * p - 7;
    return term1 * term2;
}
