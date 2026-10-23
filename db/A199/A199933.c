/*
 * A199933  Trisection 0 of A199744.
 * https://oeis.org/A199933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 5*a(n-2) + a(n-3) - a(n-4) for n > 3 a(0) = 1,
 *            a(1) = 1, a(2) = -4, a(3) = 0
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 54, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache        = NULL;   /* dynamic array of sequence values */
static size_t cache_cap    = 0;       /* allocated capacity of cache      */
static idx_t  last_computed = (idx_t)-1; /* highest index that has been computed */

/* Ensure that cache contains a(0)..a(n); grow it if necessary. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if we don't have enough space for a(n). */
    if ((size_t)n >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap : 1;
        while (newcap <= (size_t)n) {
            newcap <<= 1;          /* double until big enough */
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }

    /* Initialise the first four terms once. */
    if (last_computed == (idx_t)-1) {
        if (cache_cap < 4) {
            /* Resize to hold the first four values if necessary. */
            seq_t *tmp = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
            cache = tmp;
            cache_cap = 4;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-4;
        cache[3] = (seq_t)0;
        last_computed = 3;
    }

    /* If we already have all required values, just return. */
    if (n <= last_computed) {
        return;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = -cache[i - 1] - 5 * cache[i - 2] + cache[i - 3] - cache[i - 4];
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
