/*
 * A237262  Values of x in the solutions to x^2 - 8*x*y + y^2 + 11 = 0, where
 *          0 < x < y.
 * https://oeis.org/A237262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - a(n-4) for n > 4 Initial values: a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=15
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset by returning 0 (undefined) */
    if (n == 0) {
        return 0;
    }

    /* Static cache of previously computed terms.
       Index 1 corresponds to a(1), etc. */
    static seq_t *cache = NULL;
    static size_t cached_n = 0; /* highest index stored */

    /* Allocate and initialize the cache on first call */
    if (cache == NULL) {
        /* Allocate space for indices 1..4 (plus a dummy 0 for easy indexing) */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (cache == NULL) {
            return 0; /* allocation failure: return 0 */
        }
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 15;
        cached_n = 4;
    }

    /* If the requested term is already cached, return it immediately */
    if ((size_t)n <= cached_n) {
        return cache[n];
    }

    /* Extend the cache up to index n */
    size_t new_size = (size_t)n + 1; /* +1 to include index n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) {
        return 0; /* reallocation failed */
    }
    cache = new_cache;

    /* Compute new terms using the recurrence */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i - 2] - cache[i - 4];
    }
    cached_n = (size_t)n;

    return cache[n];
}
