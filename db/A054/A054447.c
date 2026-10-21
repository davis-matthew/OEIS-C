/*
 * A054447  Row sums of triangle A054446 (partial row sums triangle of
 *          Fibonacci convolution triangle).
 * https://oeis.org/A054447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4), * a(0) = 1, a(1)
 *            = 3, a(2) = 9, a(3) = 26 * (Derived from the generating function
 *            of the Pell‑convolution triangle)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;   /* number of allocated slots */
    static idx_t cache_max     = (idx_t)-1; /* largest n for which cache[n] is valid */

    /* initialize cache if first call */
    if (cache == NULL) {
        cache_capacity = 4; /* at least indices 0..3 */
        cache = (seq_t *)malloc((size_t)cache_capacity * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure → fatal */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)26;
        cache_max = 3;
    }

    /* if requested term already cached, return it */
    if (n <= cache_max) {
        return cache[(size_t)n];
    }

    /* grow cache to at least n+1 elements */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity;
        while (new_cap < n + 1)
            new_cap = new_cap ? new_cap * 2 : 1;   /* exponential growth, avoid 0 */

        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure → fatal */
        cache         = new_cache;
        cache_capacity = new_cap;
    }

    /* compute missing entries up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)3;
        } else if (i == 2) {
            cache[2] = (seq_t)9;
        } else if (i == 3) {
            cache[3] = (seq_t)26;
        } else {
            /* recurrence: a(i) = 4*a(i-1) - 2*a(i-2) - 4*a(i-3) - a(i-4) */
            seq_t val = 4 * cache[(size_t)(i - 1)]
                      - 2 * cache[(size_t)(i - 2)]
                      - 4 * cache[(size_t)(i - 3)]
                      -      cache[(size_t)(i - 4)];
            cache[(size_t)i] = val;
        }
    }
    cache_max = n;
    return cache[(size_t)n];
}
