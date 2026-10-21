/*
 * A229701  Squares of triangular numbers, written backwards.
 * https://oeis.org/A229701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = reverse_decimal( (n*(n+1)/2)^2 )
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 250, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n == 0 we return 0 as a safety value */
    if (n == 0) return 0;

    /* compute the n-th triangular number: n*(n+1)/2 in seq_t arithmetic */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* square it */
    seq_t sq = tn * tn;

    /* reverse the decimal digits of the square */
    return reverse_decimal(sq);
}
