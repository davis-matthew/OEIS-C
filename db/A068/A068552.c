/*
 * A068552  2n*binomial(2n,n) - 4^n.
 * https://oeis.org/A068552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2n*binomial(2n,n) - 4^n
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(2n, n) */
    seq_t bin = binomial((idx_t)(2 * n), n);

    /* 2n * binomial(2n,n) */
    seq_t term1 = ( (seq_t)n * 2 ) * bin;

    /* 4^n */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);

    /* Result */
    return term1 - pow4;
}
