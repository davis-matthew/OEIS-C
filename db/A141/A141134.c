/*
 * A141134  Hankel transform of C(2n+4,n+4).
 * https://oeis.org/A141134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = - 2*a(n-1) - 3*a(n-2) - 4*a(n-3) - 3*a(n-4) - 2*a(n-5) -
 *            a(n-6), a(0) = 1, a(1) = -8, a(2) = 8, a(3) = 1, a(4) = 1, a(5)
 *            = -16
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Static cache and control variables */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 0;   /* index of the largest value stored in cache */

    /* Helper to initialise the cache with the first six terms. */
    if (!initialized) {
        cache_size = 6;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* malloc failed – should never happen for small sizes */
        /* Base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-8;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)-16;
        last_computed = 5;
        initialized = true;
    }

    /* If the requested index is within the cached range, simply return it. */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Extend the cache if necessary. */
    if ((size_t)(n+1) > cache_size) {
        size_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* malloc failed */
        /* Copy existing values */
        for (size_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old cache and update pointers */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = -(seq_t)2 * cache[i-1];
        val -= (seq_t)3 * cache[i-2];
        val -= (seq_t)4 * cache[i-3];
        val -= (seq_t)3 * cache[i-4];
        val -= (seq_t)2 * cache[i-5];
        val -= cache[i-6];
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}
