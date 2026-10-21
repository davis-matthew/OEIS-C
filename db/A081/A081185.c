/*
 * A081185  8th binomial transform of (0,1,0,2,0,4,0,8,0,16,...).
 * https://oeis.org/A081185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 62*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure the cache holds at least indices 0 and 1 */
    if (cache_len < 2) {
        cache_len = 2;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Expand cache if required */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)16 * cache[i - 1] - (seq_t)62 * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
