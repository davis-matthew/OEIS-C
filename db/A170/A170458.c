/*
 * A170458  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A170458 – number of reduced words of length n
 *  Sequence defined by the rational generating function
 *
 *  (t^45 + 2*t^44 + 2*t^43 + … + 2*t + 1) /
 *  (120*t^45 - 15*t^44 - 15*t^43 - … - 15*t + 1)
 *
 *  For n ≥ 45 the coefficients satisfy the linear recurrence
 *
 *        a(n) = 15·(a(n-1)+a(n-2)+…+a(n-44)) – 120·a(n-45)
 *
 *  The first 45 terms are obtained from the numerator by a short
 *  preliminary computation using the same recurrence with the
 *  missing terms interpreted as zero for negative indices.
 *
 *  The implementation below builds the terms up to the requested
 *  index n, caching all intermediate values so that subsequent
 *  calls are fast.  No external helpers are used; all arithmetic
 *  is done with the type seq_t supplied by the build system.
 */

#include <stdlib.h>
#include <stddef.h>

/*  Return a(n).  The function may be called an arbitrary number of
 *  times with n in any order; all previously computed terms are kept
 *  in a static cache.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[i] holds a(i) for 0 ≤ i < cached_len */
    static size_t cached_len = 0; /* number of entries currently stored */

    /* Expand cache so that it can hold a(n).  */
    if ((size_t)(n + 1) > cached_len) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – return garbage */
        cache = tmp;

        /* Compute all terms that are not yet in the cache.  */
        for (idx_t k = cached_len; k <= n; ++k) {
            seq_t sum = 0;              /* Σ a(k-i) for i=1..44, only if k≥i */
            for (int i = 1; i <= 44 && k >= (idx_t)i; ++i)
                sum += cache[k - (idx_t)i];

            seq_t val;
            if (k == 0) {
                /* a(0) = 1 */
                val = (seq_t)1;
            } else if (k <= 44) {
                /* a(k) = 2 + 15·sum   for 1 ≤ k ≤ 44 */
                val = (seq_t)2 + (seq_t)15 * sum;
            } else if (k == 45) {
                /* a(45) = 1 + 15·sum – 120·a(0)  with the same sum over
                 * the 44 previous terms (a(1)…a(44)). */
                val = (seq_t)1 + (seq_t)15 * sum - (seq_t)120 * cache[0];
            } else {
                /* For n ≥ 46:
                 * a(n) = 15·(a(n-1)+…+a(n-44)) – 120·a(n-45) */
                val = (seq_t)15 * sum - (seq_t)120 * cache[k - 45];
            }
            cache[k] = val;
        }

        cached_len = (size_t)(n + 1);
    }

    return cache[(size_t)n];
}
