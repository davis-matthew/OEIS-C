/*
 * A227274  Positive solutions of the Pell equation x^2 - 97*y^2 = -1.
 *          Solutions x = 5604*a(n).
 * https://oeis.org/A227274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=125619267, a(n)=125619266*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;  /* number of elements stored (cache_len>=2 when initialized) */

    /* constant 2*62809633 */
    const seq_t K = (seq_t)125619266;

    /* Ensure cache is large enough */
    if (cache_len < n + 1) {
        /* Initialize cache if not yet done */
        if (cache_len == 0) {
            size_t init_cap = 2;
            cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
            if (!cache) return 0;  /* allocation failed */
            cache_cap = init_cap;
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)125619267;
            cache_len = 2;
        }

        /* Grow array if needed */
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)(n + 1)) {
                new_cap *= 2;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            size_t idx = (size_t)i;
            /* For i==0 or 1, values already set during initialization. */
            if (i == 0) {
                cache[idx] = (seq_t)1;
            } else if (i == 1) {
                cache[idx] = (seq_t)125619267;
            } else {
                cache[idx] = K * cache[idx - 1] - cache[idx - 2];
            }
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
