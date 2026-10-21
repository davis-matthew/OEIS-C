/*
 * A220753  Expansion of (1+4*x+5*x^2-x^3)/((1-x)*(1+x)*(1-2*x^2)).
 * https://oeis.org/A220753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = 7*2^n - 6, a(2n+1) = 7*2^n - 3
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n >> 1);   /* 2^floor(n/2) */
    v *= (seq_t)7;            /* 7 * 2^floor(n/2) */
    if ((n & 1) == 0)         /* n even: subtract 6 */
        v -= (seq_t)6;
    else                      /* n odd: subtract 3 */
        v -= (seq_t)3;
    return v;
}
