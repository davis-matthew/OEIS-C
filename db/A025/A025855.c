/*
 * A025855  Expansion of 1/((1-x^3)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A025855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025855: number of partitions of n into parts 3, 9, and 11.
   Recurrence: a(n) = a(n‑3) + a(n‑9) + a(n‑11) – a(n‑12) – a(n‑14) – a(n‑20) + a(n‑23)
   Initial values: a(0) = 1, a(1) = 0, a(2) = 0. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;   /* number of allocated slots */
    static size_t cache_last   = (size_t)-1;   /* last index that has been computed */

    size_t need = (size_t)(n + 1);

    /* Grow the cache if necessary. */
    if (need > cache_alloc) {
        seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_cache)                /* allocation failed – return 0 */
            return (seq_t)0;
        cache = new_cache;
        cache_alloc = need;

        /* Initialise the first few terms on the very first allocation. */
        if (cache_last == (size_t)-1) {
            if (need > 0)   cache[0] = (seq_t)1;
            if (need > 1)   cache[1] = (seq_t)0;
            if (need > 2)   cache[2] = (seq_t)0;
            cache_last = (need < 3) ? (need - 1) : 2;
        }
    }

    /* Compute any missing terms up to index n. */
    if (n > cache_last) {
        for (size_t i = cache_last + 1; i <= n; ++i) {
            seq_t sum = 0;
            if (i >= 3)  sum += cache[i - 3];
            if (i >= 9)  sum += cache[i - 9];
            if (i >= 11) sum += cache[i - 11];
            if (i >= 12) sum -= cache[i - 12];
            if (i >= 14) sum -= cache[i - 14];
            if (i >= 20) sum -= cache[i - 20];
            if (i >= 23) sum += cache[i - 23];
            cache[i] = sum;
        }
        cache_last = n;
    }

    return cache[n];
}
