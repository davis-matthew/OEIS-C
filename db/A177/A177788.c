/*
 * A177788  a(n) = binomial(n^2, n+1)/(n-1).
 * https://oeis.org/A177788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n^2, n+1)/(n-1) = n * binomial(n^2-2, n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 2.  For other values return 0. */
    if (n < 2)
        return 0;

    /* N = n^2 - 2, the upper parameter of the binomial coefficient. */
    seq_t N = (seq_t)n * (seq_t)n - 2;

    /* Compute binomial(N, n-1) multiplicatively.  The intermediate
       results are always integers because the standard binomial product
       expression keeps the division exact at each step. */
    seq_t binom = 1;
    for (idx_t k = 1; k < n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t term = N - (seq_t)(k - 1);  /* N - k + 1 */
        binom = binom * term / kseq;
    }

    /* a(n) = n * binomial(n^2-2, n-1) */
    return binom * (seq_t)n;
}
