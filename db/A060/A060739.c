/*
 * A060739  a(n) = (-1)^(n*(n-1)/2) * Product_{k=0..n-1} (n+k-1)!/((k!)^2 *
 *          (n-1-k)!).
 * https://oeis.org/A060739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, nice
 * Formula:   a(n) = (-1)^(n*(n-1)/2) * Product_{k=1}^{n-1} binomial(2*k,k)^2
 *            / 2
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 9,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* here binom(a,b) is provided in ../utils/binom.h                         */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0) = 1 */

    seq_t prod = 1;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t b = binom(2 * k, k);          /* binomial(2k,k) */
        seq_t term = (b * b) / 2;            /* (binomial)^2 / 2  */
        prod = prod * term;
    }

    /* sign = (-1)^{n*(n-1)/2} */
    seq_t exp = ((seq_t)n * ((seq_t)n - 1)) / 2;
    if (exp & 1)
        prod = -prod;

    return prod;
}
