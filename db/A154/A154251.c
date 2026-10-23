/*
 * A154251  Expansion of (1-x+7x^2)/((1-x)(1-2x)).
 * https://oeis.org/A154251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^(n-1) - 7 for n > 0; a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
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
    seq_t v = pow2(n - 1);      /* v = 2^(n-1) */
    v = v * (seq_t)9;           /* v = 9 * 2^(n-1) */
    v = v - (seq_t)7;           /* v = 9*2^(n-1) - 7 */
    return v;
}
