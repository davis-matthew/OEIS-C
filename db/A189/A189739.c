/*
 * A189739  a(1)=3, a(2)=5, a(n)=3*a(n-1) + 5*a(n-2).
 * https://oeis.org/A189739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2), a(1)=3, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/**
 * Compute a(n) of OEIS A189739.
 *
 * Uses memoization with a static cache so that calls in any order
 * are fast and use only O(1) additional overhead per call.
 */
seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 1) return 3;
    if (n == 2) return 5;

    /* static cache: index 0 unused; cache[1] = a(1), cache[2] = a(2) */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* initialise cache if this is the first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = 0;                     /* unused */
        cache[1] = 3;                     /* a(1) */
        cache[2] = 5;                     /* a(2) */
    }

    /* grow cache if needed and compute missing terms */
    if (n > cache_cap) {
        idx_t old_cap = cache_cap;
        /* increase to accommodate indices up to n */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_cap = n;
        /* compute new values */
        for (idx_t i = old_cap + 1; i <= n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 1] + (seq_t)5 * cache[i - 2];
        }
    }

    /* the requested value is now in the cache */
    return cache[n];
}
