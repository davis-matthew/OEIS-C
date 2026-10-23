/*
 * A153079  a(n) = 13^n + 2.
 * https://oeis.org/A153079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13^n + 2 Recurrence: a(0) = 3; a(1) = 15; a(n) =
 *            14*a(n-1) - 13*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (thread‑safe in the OEIS test harness) */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity */
    static size_t cache_size = 0;   /* number of elements that have been computed */

    /* initialise the cache when first called */
    if (cache == NULL) {
        cache_cap = 16;                     /* arbitrary start size */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure unlikely */

        /* initialise first two terms */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)15;
        cache_size = 2;
    }

    /* convert the caller's index to a size_t for array indexing    */
    size_t idx = (size_t)n;

    /* Grow storage if the requested index is beyond the current capacity */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (idx >= new_cap) new_cap <<= 1;      /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing entries up to idx */
    for (size_t i = cache_size; i <= idx; ++i) {
        if (i >= 2) {
            /* a(i) = 14*a(i-1) – 13*a(i-2) */
            cache[i] = (seq_t)14 * cache[i-1] - (seq_t)13 * cache[i-2];
        }
        /* i==0 or i==1 are already initialised */
    }

    /* Update the size of the computed range if we grew it */
    if (idx + 1 > cache_size) cache_size = idx + 1;

    return cache[idx];
}
