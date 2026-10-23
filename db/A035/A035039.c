/*
 * A035039  a(n) = 2^n - C(n,0) - C(n,1) - ... - C(n,6).
 * https://oeis.org/A035039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + C(n-1,6), a(0)=a(1)=...=a(6)=0 (C(n,k) =
 *            n!/(k!(n-k)!)). Uses memoisation; O(n) time per call for the
 *            maximum n requested. Assumes seq_t supports +, *, / operators
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* array of a values */
    static idx_t cache_len = 0;     /* allocated length of cache */
    static idx_t max_computed = 0;  /* highest index whose value is ready */

    /* Initialise cache on first call */
    if (!cache) {
        cache_len = (idx_t)(n + 1);
        cache = (seq_t*)calloc(cache_len, sizeof(seq_t));
        /* a(0) = 0 already */
        max_computed = 0;
    }

    /* Grow cache if necessary */
    if (n + 1 > cache_len) {
        cache_len = (idx_t)(n + 1);
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* Compute any missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t binom = 0;
            if (i > 6) {            /* binomial only defined for i-1 >= 6 */
                idx_t t = i - 1;
                seq_t prod = (seq_t)t;
                prod *= (seq_t)(t - 1);
                prod *= (seq_t)(t - 2);
                prod *= (seq_t)(t - 3);
                prod *= (seq_t)(t - 4);
                prod *= (seq_t)(t - 5);
                binom = prod / (seq_t)720;  /* 6! = 720 */
            }
            cache[i] = cache[i - 1] * (seq_t)2 + binom;
        }
        max_computed = n;
    }

    return cache[n];
}
