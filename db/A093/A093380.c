/*
 * A093380  Expansion of (1+4x+x^2-10x^3)/((1-x)(1-x-2x^2)).
 * https://oeis.org/A093380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 2 + 4*(2^(n+1) + (-1)^n)/3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^(n+1) */
    seq_t pow = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow *= 2;

    /* Add (-1)^n */
    if ((n & 1) == 0)      /* n even */
        pow += (seq_t)1;
    else                   /* n odd */
        pow -= (seq_t)1;   /* pow is at least 4 for n ≥ 1, so no underflow */

    /* Divide by 3 (the result is always an integer) */
    seq_t t_div3 = pow / (seq_t)3;

    /* Final formula: 2 + 4 * t_div3 */
    return (seq_t)2 + t_div3 * (seq_t)4;
}
