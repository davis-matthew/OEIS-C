/*
 * A133570  "Exact" continued fraction of e.
 * https://oeis.org/A133570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 126, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A133570: Exact continued fraction of e
   Recurrence: a(n) = -2*a(n-2) - a(n-4)    for n > 5
   Initial values: a(0)=3, a(1)=-4, a(2)=2,
                   a(3)=5, a(4)=-2, a(5)=-7
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* current allocated size       */
    static idx_t cache_used = 0;  /* number of values already set */

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed – return 0 as a sentinel */
        /* copy old contents */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* free old storage */
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise the first six terms once */
    if (cache_used < 6) {
        static const seq_t init[6] = {3, -4, 2, 5, -2, -7};
        idx_t upto = n < 5 ? n : 5;
        for (idx_t i = cache_used; i <= upto; ++i)
            cache[i] = init[i];
        cache_used = (upto + 1 > cache_used) ? (upto + 1) : cache_used;
    }

    /* compute missing terms using the recurrence, but only for n >= 6 */
    if (n >= 6 && cache_used <= n) {
        idx_t start = cache_used > 6 ? cache_used : 6;
        for (idx_t i = start; i <= n; ++i) {
            /* recurrence: a(i) = -2*a(i-2) - a(i-4)   for i >= 6 */
            cache[i] = -(seq_t)2 * cache[i - 2] - cache[i - 4];
        }
        cache_used = n + 1;
    }

    return cache[(idx_t)n];
}
