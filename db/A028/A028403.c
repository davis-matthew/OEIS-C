/*
 * A028403  Number of types of Boolean functions of n variables under a
 *          certain group.
 * https://oeis.org/A028403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2^(n-1) + 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; treat n==0 as undefined, return 0 */
    if (n == 0)
        return 0;

    seq_t p_n   = pow2(n);     /* 2^n */
    seq_t p_n_1 = pow2(n - 1); /* 2^(n-1) */

    p_n_1 += 1;                /* 2^(n-1) + 1 */
    return p_n_1 * p_n;        /* 2^n * (2^(n-1) + 1) */
}
