/*
 * A151392  Duplicate of A000888.
 * https://oeis.org/A151392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = binom(2*n, n)^2/(n+1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* central binomial coefficient C(2n,n) */
    seq_t c = binom(2 * n, n);

    /* compute C(2n,n)^2 */
    seq_t prod = c * c;

    /* divide by (n+1) */
    return prod / ((seq_t)n + 1);
}
