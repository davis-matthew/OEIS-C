/*
 * A139701  Binomial transform of [1, 100, 100, 100, ...].
 * https://oeis.org/A139701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*2^(n-1) - 99 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n==0, return 0 safely */
        return 0;

    seq_t v = pow2(n - 1);        /* v = 2^(n-1) in seq_t */
    v = v * (seq_t)100;           /* v = 100 * 2^(n-1) */
    v = v - (seq_t)99;            /* v = 100*2^(n-1) - 99 */
    return v;
}
