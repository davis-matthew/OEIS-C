/*
 * A016296  Expansion of g.f. 1/((1 - 2*x)*(1 - 5*x)*(1 - 7*x)).
 * https://oeis.org/A016296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 59*a(n-2) + 70*a(n-3) (n ≥ 3) Initial values:
 *            a(0)=1, a(1)=14, a(2)=137
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static idx_t  cache_len  = 0;      /* number of initialised terms   */
    static idx_t  cache_cap  = 0;      /* allocated capacity            */

    /* Allocate and initialise cache on first call. */
    if (cache_len == 0) {
        /* Ensure we have room for the first three values. */
        if (cache_cap < 4) {
            cache_cap = 4;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)137;
        cache_len = 3;
    }

    /* Expand cache if necessary. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= n) new_cap <<= 1;              /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { /* already initialised */ }
        else if (i == 1) { /* already initialised */ }
        else if (i == 2) { /* already initialised */ }
        else
            cache[i] = (seq_t)14 * cache[i-1]
                     - (seq_t)59 * cache[i-2]
                     + (seq_t)70 * cache[i-3];
    }
    cache_len = n + 1;
    return cache[n];
}
