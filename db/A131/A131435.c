/*
 * A131435  Recursive sequence generated from a Petersen graph.
 * https://oeis.org/A131435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 15*a(n-2) + 3*a(n-3) - 13*a(n-4) + 4*a(n-5)
 *            with initial values: a(1) = 1, a(2) = 6, a(3) = 35, a(4) = 198,
 *            a(5) = 1124
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization.  cache[1]..cache[cache_size] hold known values. */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* largest n that has been computed */
    static idx_t  cache_cap   = 0;   /* allocated size of cache (unrelated to cache_size) */

    /* Initial values (only compute once) */
    if (cache == NULL) {
        /* Allocate space for indices 0..5 (index 0 unused) */
        cache_cap   = 6;
        cache       = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[1] = 1;
        cache[2] = 6;
        cache[3] = 35;
        cache[4] = 198;
        cache[5] = 1124;
        cache_size = 5;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_size) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        /* Allocate at least enough for n+1 elements */
        idx_t new_cap = n + 1;
        /* allocate new array */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* realloc may fail; in that unlikely case we let it propagate as a crash */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1]
                 + 15 * cache[i - 2]
                 + 3 * cache[i - 3]
                 - 13 * cache[i - 4]
                 + 4 * cache[i - 5];
    }

    cache_size = n;
    return cache[n];
}
