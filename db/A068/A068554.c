/*
 * A068554  a(n) = n*binomial(2n, n) - 4^n.
 * https://oeis.org/A068554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*binomial(2n, n) - 4^n
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 30, i128 n <= 62,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute 4^n */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;

    /* central binomial coefficient C(2n, n) */
    seq_t bin = binom(n << 1, n);

    /* a(n) = n * bin - 4^n */
    return ((seq_t)n) * bin - pow4;
}
