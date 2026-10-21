/*
 * A238462  2-adic valuation of A052129.
 * https://oeis.org/A238462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + v_2(n), with a(0) = 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here v_2(n) is the exponent of the largest power of 2 dividing n */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache holding values a(0)..a(cache_len-1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache on first call */
    if (cache_len == 0) {
        idx_t size = n + 1;                    /* n may be 0 */
        cache = (seq_t*)calloc(size, sizeof(seq_t));
        cache_len = size;
        cache[0] = (seq_t)0;                   /* a(0) = 0 */

        for (idx_t k = 1; k <= n; ++k) {
            idx_t m = k;
            int v2 = 0;
            while ((m & 1ULL) == 0) {
                ++v2;
                m >>= 1;
            }
            cache[k] = cache[k-1] * (seq_t)2 + (seq_t)v2;
        }
        return cache[n];
    }

    /* Return already computed value */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache to include up to n */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    cache_len = new_len;

    for (idx_t k = old_len; k <= n; ++k) {
        idx_t m = k;
        int v2 = 0;
        while ((m & 1ULL) == 0) {
            ++v2;
            m >>= 1;
        }
        cache[k] = cache[k-1] * (seq_t)2 + (seq_t)v2;
    }

    return cache[n];
}
