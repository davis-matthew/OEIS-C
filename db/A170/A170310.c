/*
 * A170310  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let p[n] = 1 (n==0), 2 (1 ≤ n ≤ 41), 1 (n==42), 0 otherwise.
 *            Then for all n ≥ 0: a[n] = p[n] + 11 * Σ_{i=1}^{min(n,41)}
 *            a[n-i] – (n ≥ 42 ? 66 * a[n-42] : 0) with a[0] = 1. This follows
 *            from the rational generating function given in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Allocate or grow the cache as needed */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute terms sequentially from the previous length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            /* Add 11 * a[i-1] + ... + 11 * a[i-41] if applicable */
            idx_t bound = (i < 41) ? i : 41;
            for (idx_t k = 1; k <= bound; ++k) {
                sum += (seq_t)11 * cache[i - k];
            }

            /* Subtract 66 * a[i-42] when i ≥ 42 */
            if (i >= 42) {
                sum -= (seq_t)66 * cache[i - 42];
            }

            /* Determine p[i] */
            seq_t p = 0;
            if (i == 0) {
                p = 1;
            } else if (i <= 41) {
                p = 2;
            } else if (i == 42) {
                p = 1;
            } /* else p remains 0 */

            cache[i] = p + sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
