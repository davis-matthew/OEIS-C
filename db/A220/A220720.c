/*
 * A220720  Number of ways to reciprocally link elements of an n X 3 array
 *          either to themselves or to exactly two horizontal, diagonal or
 *          antidiagonal neighbors.
 * https://oeis.org/A220720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) + 5*a(n-3) + 3*a(n-4) - 2*a(n-5) *
 *            Recurrence base: a(1)=1, a(2)=7, a(3)=30, a(4)=126, a(5)=534
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* Number of cache entries (index 0 unused). */

    /* Base values for n = 1..5. */
    static const seq_t base[6] = {0, 1, 7, 30, 126, 534};

    /* Return base case directly if requested. */
    if (n <= 5)
        return base[n];

    /* Allocate or expand cache if necessary. */
    if (cache == NULL) {
        cache_size = n + 1;                         /* +1 so that index n is usable. */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                      /* In case of allocation failure. */

        /* Store base values in the cache. */
        cache[1] = base[1];
        cache[2] = base[2];
        cache[3] = base[3];
        cache[4] = base[4];
        cache[5] = base[5];

        /* Compute all terms up to n. */
        for (idx_t i = 6; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1]
                     + 4 * cache[i - 2]
                     + 5 * cache[i - 3]
                     + 3 * cache[i - 4]
                     - 2 * cache[i - 5];
        }
        return cache[n];
    }

    if ((size_t)n < cache_size)
        return cache[n];

    /* Extend cache to accommodate up to n. */
    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0;                       /* Allocation failure. */
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1]
                 + 4 * cache[i - 2]
                 + 5 * cache[i - 3]
                 + 3 * cache[i - 4]
                 - 2 * cache[i - 5];
    }
    cache_size = new_size;
    return cache[n];
}
