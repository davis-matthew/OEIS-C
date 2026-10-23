/*
 * A134169  a(n) = 2^(n-1)*(2^n - 1) + 1.
 * https://oeis.org/A134169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^n - 1) + 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p2n  = pow2(n);      /* 2^n   */
    seq_t p2n1 = pow2(n - 1);  /* 2^(n-1) */
    return p2n1 * (p2n - (seq_t)1) + (seq_t)1;
}
