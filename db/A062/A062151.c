/*
 * A062151  Fifth column of triangle A062138 (generalized a=5 Laguerre).
 * https://oeis.org/A062151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)! * binomial(n+9,9) / 4!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t val = 1;
    /* (n+4)! / 4! = ∏_{k=5}^{n+4} k */
    for (idx_t k = 5; k <= n + 4; ++k) {
        val *= (seq_t)k;
    }
    seq_t bin = binom(n + 9, 9);
    val *= bin;
    return val;
}
