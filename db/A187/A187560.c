/*
 * A187560  a(n) = 4^(n+1)-2^n-1.
 * https://oeis.org/A187560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n+1) - 2^n - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n once */
    seq_t pow2_n = pow2(n);

    /* 4^(n+1) = 2^(2*(n+1)) = 2^(2*n + 2) */
    seq_t res = pow2(((idx_t)n) * 2 + 2);

    /* Subtract the terms as per the formula */
    res -= pow2_n;
    res -= 1;

    return res;
}
