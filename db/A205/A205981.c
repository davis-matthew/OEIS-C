/*
 * A205981  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 60*a(n-3) + 8*a(n-5) for n>9, with initial values:
 *            a(1)=2232, a(2)=8004, a(3)=27060, a(4)=103998, a(5)=408411,
 *            a(6)=1599096, a(7)=6296976, a(8)=24685680, a(9)=96780240
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_nmax = 0;   /* highest n for which we have stored a(n) */
    static size_t cache_cap = 0;    /* maximum index currently allocated */

    /* Initialise cache with the first nine terms when first called */
    if (cache == NULL) {
        cache_cap = 10;             /* allocate space for indices 0..10 (0 unused) */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure: return zero as fallback */

        /* zero all entries */
        for (size_t i = 0; i <= cache_cap; ++i) cache[i] = (seq_t)0;

        /* known initial values */
        static const seq_t init[10] = {
            0,          /* unused */
            2232,       /* a(1) */
            8004,       /* a(2) */
            27060,      /* a(3) */
            103998,     /* a(4) */
            408411,     /* a(5) */
            1599096,    /* a(6) */
            6296976,    /* a(7) */
            24685680,   /* a(8) */
            96780240    /* a(9) */
        };
        for (idx_t i = 1; i <= 9 && i <= cache_cap; ++i)
            cache[i] = init[i];
        cached_nmax = 9;
    }

    /* If requested n is already cached, return it */
    if (n <= cached_nmax) return cache[n];

    /* Grow cache if necessary */
    if ((idx_t)(cache_cap) < n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: fallback to zero */
        cache = tmp;
        /* zero newly allocated entries */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cached_nmax + 1; i <= n; ++i) {
        if (i <= 9) {
            /* should not happen because we already initialised up to 9 */
            continue;
        }
        /* a(i) = 60*a(i-3) + 8*a(i-5) */
        cache[i] = cache[i - 3] * (seq_t)60 + cache[i - 5] * (seq_t)8;
    }

    cached_nmax = n;
    return cache[n];
}
