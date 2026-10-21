/*
 * A095121  Expansion of (1-x+2x^2)/((1-x)*(1-2x)).
 * https://oeis.org/A095121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*2^n - 2 + (n==0?1:0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute A095121: Expansion of (1-x+2x^2)/((1-x)*(1-2x)) */
seq_t a(idx_t n)
{
    /* 2^n */
    seq_t p = pow2(n);

    /* 2 * 2^n */
    seq_t res = p * (seq_t)2;

    /* subtract 2 */
    res -= (seq_t)2;

    /* add 1 for n==0 (since 0^0 = 1) */
    if (n == 0)
        res += (seq_t)1;

    return res;
}
