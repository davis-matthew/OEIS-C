/*
 * A098972  a(0) = 1; for n > 0, a(n) = 2*a(floor(n/2)) + 3*a(floor(n/3)).
 * https://oeis.org/A098972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 287, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements the recurrence defining OEIS A098972:
   a(0) = 1;
   a(n) = 2 * a(floor(n/2)) + 3 * a(floor(n/3))   for n > 0.
   The function computes values iteratively and caches them so that
   repeated calls (in any order) are efficient. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache: cache[i] holds a(i) for i < computed. */
    static seq_t *cache = NULL;
    static size_t cache_size  = 0; /* allocated size */
    static size_t computed    = 0; /* highest index computed + 1 */

    size_t idx = (size_t)n;           /* local copy for array indexing */

    /* Expand the cache if necessary. */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;    /* need indices 0 .. idx */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();     /* allocation failure */

        /* Copy existing computed values. */
        for (size_t i = 0; i < computed; ++i)
            new_cache[i] = cache[i];
        /* Zero initialize the remaining slots (not really needed). */
        for (size_t i = computed; i < new_size; ++i)
            new_cache[i] = (seq_t)0;

        /* Replace old cache. */
        free(cache);
        cache       = new_cache;
        cache_size  = new_size;
    }

    /* Compute missing terms up to idx. */
    if (idx >= computed) {
        for (size_t i = computed; i <= idx; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                cache[i] = 2 * cache[i/2] + 3 * cache[i/3];
            }
        }
        computed = idx + 1;   /* now we have values up to idx */
    }

    return cache[idx];
}
