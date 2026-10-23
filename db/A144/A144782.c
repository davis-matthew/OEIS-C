/*
 * A144782  Variant of Sylvester's sequence: a(n+1) = a(n)^2 - a(n) + 1, with
 *          a(1) = 9.
 * https://oeis.org/A144782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 9, a(n+1) = a(n)^2 - a(n) + 1 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        /* undefined for n < 1; return 0 as a safe placeholder */
        return (seq_t)0;
    }

    /* static cache for memoisation */
    static seq_t *cache  = NULL;  /* cache[0] holds a(1) */
    static idx_t  cache_max = 0;  /* highest n already computed   */
    static size_t cache_cap = 0;  /* number of slots allocated    */

    /* return cached value if available */
    if (n <= cache_max) {
        return cache[n - 1];
    }

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate the program */
            abort();
        }
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term if the cache was empty */
    if (cache_max == 0) {
        cache[0] = (seq_t)9;   /* a(1) */
        cache_max = 1;
    }

    /* compute terms from cache_max+1 up to n inclusive */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */
        seq_t curr = prev * prev - prev + 1;
        cache[i - 1] = curr;                /* store a(i) */
    }

    cache_max = n;
    return cache[n - 1];
}
