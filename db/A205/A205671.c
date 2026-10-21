/*
 * A205671  E.g.f. A(x) = Sum_{n>0} a(n)*x^n/n! is the inverse function to
 *          exp(2*x)-x-1.
 * https://oeis.org/A205671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
  Sequence A205671
  Recurrence:  a(1) = 1
                a(n) = 2*(1 - n)*a(n-1) - ∑_{j=1}^{n-1} binomial(n,j)*a(j)*a(n-j)   (n > 1)
  Computes terms for n ≥ 1, using dynamic programming and memoisation.
*/
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – return 0 as a safe fallback. */
    if (n == 0) return 0;

    /* Static cache for memoisation */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;
    static idx_t  len    = 0;

    /* Expand cache if required. */
    if (n > cap) {
        idx_t newcap = cap ? cap : 1;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t m = len + 1; m <= n; ++m) {
        if (m == 1) {
            cache[0] = (seq_t)1;                     /* a(1) = 1 */
        } else {
            /* Compute the summation part.
               binom = binomial(m, j) computed iteratively.             */
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;    /* binomial(m,0) */

            for (idx_t j = 1; j < m; ++j) {
                /* Update binom = binomial(m,j) from binomial(m,j-1). */
                binom = binom * (seq_t)(m - j + 1) / (seq_t)j;

                seq_t prod = binom * cache[j - 1] * cache[m - j - 1];
                sum += prod;
            }

            /* Compute the coefficient term 2*(1 - m)*a(m-1). */
            seq_t coeff = (seq_t)2 * ((seq_t)1 - (seq_t)m) * cache[m - 2];

            cache[m - 1] = coeff - sum;       /* a(m) */
        }
    }

    len = n;
    return cache[n - 1];
}
