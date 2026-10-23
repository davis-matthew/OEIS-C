/*
 * A122050  a(n) = (n-1)*a(n-1) - a(n-4) with a(0)=0, a(1)=1, a(2)=2, a(3)=1.
 * https://oeis.org/A122050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1) - a(n-4), a(0)=0, a(1)=1, a(2)=2, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A simple memoisation using a static array */
    static seq_t *cache      = NULL;   /* cached values a(0) … a(cache_size-1) */
    static size_t cache_size = 0;      /* number of cached values */

    /* If already computed, return immediately */
    if (n < cache_size) {
        return cache[n];
    }

    /* Need to extend the cache to hold values up to n */
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failed – return a zero value (undefined behaviour in
           an actual build would never happen, but we guard against it). */
        return (seq_t)0;
    }

    /* Copy previously computed values */
    for (size_t i = 0; i < cache_size; ++i) {
        new_cache[i] = cache[i];
    }

    /* Compute new entries from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            new_cache[i] = (seq_t)0;
        } else if (i == 1) {
            new_cache[i] = (seq_t)1;
        } else if (i == 2) {
            new_cache[i] = (seq_t)2;
        } else if (i == 3) {
            new_cache[i] = (seq_t)1;
        } else {
            /* a(i) = (i-1)*a(i-1) - a(i-4) */
            seq_t mult = (seq_t)(i - 1) * new_cache[i - 1];
            new_cache[i] = mult - new_cache[i - 4];
        }
    }

    /* Replace old cache */
    free(cache);
    cache      = new_cache;
    cache_size = new_size;

    return cache[n];
}
