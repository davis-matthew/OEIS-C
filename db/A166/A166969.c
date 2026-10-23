/*
 * A166969  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A166969 – number of reduced words of length n in the
    Coxeter group on 14 generators with S_i^2 = (S_i S_j)^13 = I.
    The generating function yields the linear recurrence

        a(0) = 1
        a(n) = 2 + 12 * Σ_{k=0}^{n-1} a(k)          for 1 ≤ n ≤ 12
        a(13) = 1 + 12 * Σ_{k=1}^{12} a(13-k) - 78 * a(0)
        a(n) = 12 * Σ_{k=1}^{12} a(n-k) - 78 * a(n-13)   for n ≥ 14
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed values */
    static idx_t cache_size = 0;  /* number of stored terms (0 .. cache_size-1) */

    /* Grow the cache up to index n (inclusive) */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (idx_t m = cache_size; m <= n; ++m) {
            seq_t sum = 0;

            if (m == 0) {
                cache[0] = 1;
                continue;
            }

            /* 1 ≤ m ≤ 12 : use the first recurrence */
            if (m <= 12) {
                for (idx_t k = 0; k < m; ++k)
                    sum += cache[k];
                cache[m] = 2 + (seq_t)12 * sum;
                continue;
            }

            /* m == 13 : special case */
            if (m == 13) {
                for (idx_t k = 1; k <= 12; ++k)
                    sum += cache[m - k];
                cache[13] = 1 + (seq_t)12 * sum - (seq_t)78 * cache[0];
                continue;
            }

            /* m ≥ 14 : general recurrence */
            for (idx_t k = 1; k <= 12; ++k)
                sum += cache[m - k];
            cache[m] = (seq_t)12 * sum - (seq_t)78 * cache[m - 13];
        }

        cache_size = n + 1;
    }

    return cache[n];
}
