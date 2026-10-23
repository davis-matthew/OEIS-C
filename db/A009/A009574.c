/*
 * A009574  Expansion of e.g.f. sinh(log(1+x))*exp(x).
 * https://oeis.org/A009574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(0) = 0;  a(n) = -n*a(n-1) + binomial(n+1,2)  (n ≥ 1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if ((idx_t)cache_size <= n) {
        /* compute up to n, extending the cache */
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* memory allocation failure */
        cache = new_cache;

        /* fill new entries */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else {
                /* binomial(i+1,2) = (i+1)*i/2 */
                seq_t binom = ((seq_t)i * (seq_t)(i + 1)) / 2;
                cache[i] = - ((seq_t)i * cache[i - 1]) + binom;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
