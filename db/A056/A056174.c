/*
 * A056174  Number of non-monotone maps from 1,...,n to 1,...,n.
 * https://oeis.org/A056174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - C(2n, n) + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute n^n */
    seq_t pow_n_n = big_pow((seq_t)n, (seq_t)n);
    /* compute binomial coefficient C(2n,n) */
    seq_t binom = binomial(2 * n, n);
    /* return the result */
    return pow_n_n - binom + n;
}
