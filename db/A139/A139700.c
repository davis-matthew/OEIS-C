/*
 * A139700  Binomial transform of [1, 30, 30, 30, ...].
 * https://oeis.org/A139700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*2^(n-1) - 29. a(1)=1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^(n-1) – the pow2 helper returns a seq_t */
    seq_t v = pow2(n - 1);

    /* multiply by 30 and subtract 29 */
    v *= (seq_t)30;
    v -= (seq_t)29;

    return v;
}
