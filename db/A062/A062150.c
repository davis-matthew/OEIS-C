/*
 * A062150  Fourth (unsigned) column of triangle A062138 (generalized a=5
 *          Laguerre).
 * https://oeis.org/A062150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)! * binomial(n+8,8) / 3!
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"       /* provides seq_t binom(idx_t a, idx_t b) */

/* compute a(n) - OEIS A062150 */
seq_t a(idx_t n)
{
    /* (n+3)! */
    seq_t fact = 1;
    idx_t m = n + 3;          /* n+3 */
    for (idx_t i = 2; i <= m; ++i)
        fact *= i;

    /* binomial(n+8, 8) */
    seq_t bin = binom(n + 8, 8);

    /* combine: (n+3)! * C(n+8,8) / 6 */
    return (fact * bin) / 6;
}
