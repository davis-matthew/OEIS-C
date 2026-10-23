/*
 * A172163  a(n) = ( A165155(n) - A165154(n) )/2.
 * https://oeis.org/A172163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A172163
   Formula: a(n) = 102*a(n-1) - 101*a(n-2) - 9900*a(n-3)
   Initial values: a(0)=0, a(1)=0, a(2)=10
   (Recurrence valid for n > 2) */

#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;
static idx_t cache_len = 0;       /* first unused index (size of data stored) */
static size_t cache_cap = 0;      /* allocated capacity of cache */

/* Ensure that the cache contains all values up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)            /* already available */
        return;

    /* Number of elements that must be stored */
    size_t needed = ((size_t)n) + 1;

    /* Grow the cache if required */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)            /* allocation failed */
            exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute and store new terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = 0;
        } else if (i == 1) {
            cache[i] = 0;
        } else if (i == 2) {
            cache[i] = 10;
        } else {
            cache[i] = 102 * cache[i - 1]
                       - 101 * cache[i - 2]
                       - 9900 * cache[i - 3];
        }
    }
    cache_len = needed;
}

/* Public interface required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
