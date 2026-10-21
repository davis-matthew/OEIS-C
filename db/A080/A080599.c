/*
 * A080599  Expansion of e.g.f.: 2/(2-2*x-x^2).
 * https://oeis.org/A080599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = n * a(n-1) + (n*(n-1)/2) * a(n-2), with a(0) = 1, a(1) =
 *            1. The recurrence is computed iteratively and memoised so that
 *            calls may be made in any order
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- static cache for memoisation --------------------------------- */
    static seq_t *cache = NULL;      /* array of computed terms            */
    static idx_t  cache_len = 0;     /* number of cached terms            */

    /* --- initialise cache with the first two terms on first call --- */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0; /* defensive: unlikely to happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* --- return cached value if already known ------------------------ */
    if (n < cache_len)
        return cache[n];

    /* --- grow cache to accommodate up to n -------------------------- */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* defensive: unlikely to happen */
        cache = new_cache;

        /* --- compute missing terms from current end up to n --------- */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* term1 = i * a(i-1) */
            seq_t term1 = (seq_t)i * cache[i-1];

            /* small = i*(i-1)/2  (fits in idx_t for all i <= 10000) */
            idx_t small = (i * (i - 1)) / 2;
            /* term2 = small * a(i-2) */
            seq_t term2 = (seq_t)small * cache[i-2];

            cache[i] = term1 + term2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
