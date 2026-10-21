/*
 * A052958  Expansion of g.f.: (1-x)/(1-3*x-2*x^3+2*x^4).
 * https://oeis.org/A052958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-3) - 2*a(n-4) with a(0)=1, a(1)=2,
 *            a(2)=6, a(3)=20. This implements the OEIS sequence A052958
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array of computed values   */
    static size_t cache_len = 0;    /* number of valid entries  */
    static size_t cache_cap = 0;    /* allocated capacity         */

    /* Return cached value if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise cache with the first four terms if it hasn't been done yet */
    if (cache_len == 0) {
        size_t init_cap = 4;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)20;
        cache_len = 4;
        cache_cap = init_cap;
    }

    /* Grow the cache if larger n is requested */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4) {
            /* Base terms already present, nothing to do */
            continue;
        }
        /* a(i) = 3*a(i-1) + 2*a(i-3) - 2*a(i-4) */
        seq_t val = (seq_t)3 * cache[i - 1]
                  + (seq_t)2 * cache[i - 3]
                  - (seq_t)2 * cache[i - 4];
        cache[i] = val;
    }

    /* Update cache size information */
    cache_len = n + 1;

    /* Return requested value */
    return cache[n];
}
