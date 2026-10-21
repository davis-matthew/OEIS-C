/*
 * A087213  Expansion of (1+x-4*x^2) / ((1+x)*(1-4*x^2)).
 * https://oeis.org/A087213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=0,
   for n>2: a(n) = -a(n-1) + 4*a(n-2) + 4*a(n-3) */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* number of allocated entries */

    /* initialize the cache on the first call */
    if (cache_cap == 0) {
        cache_cap = 3;                     /* we need at least a(0), a(1), a(2) */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* fallback: out‑of‑memory → 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
    }

    /* if the requested index is already cached, return it */
    if ((size_t)n < cache_cap)
        return cache[n];

    /* grow the cache to accommodate indices up to n */
    size_t old_cap = cache_cap;
    cache_cap = (size_t)n + 1;            /* ensure room for a[n] */
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) return 0;                   /* fallback: out‑of‑memory → 0 */
    cache = tmp;

    /* compute new terms using the recurrence */
    size_t i;
    for (i = old_cap; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1 || i == 2) {
            cache[i] = (seq_t)0;
        } else {
            cache[i] = -cache[i - 1]
                     + (seq_t)4 * cache[i - 2]
                     + (seq_t)4 * cache[i - 3];
        }
    }

    return cache[n];
}
