/*
 * A015498  a(0)=1, a(1)=10, a(n) = sum_{k=0}^{k=n-1} 10^k a(k).
 * https://oeis.org/A015498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{n-1} 10^k · a(k) Initial values: a(0)=1,
 *            a(1)=10, a(2)=101 For n≥3: a(n) = (10^(n-1)+1) · a(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* If we already have the value, return it immediately */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Initialise the cache if it is empty */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
    }

    /* Grow the cache to accommodate up to n */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            idx_t idx = (idx_t)i;
            if (idx == 0) {
                cache[i] = (seq_t)1;
            } else if (idx == 1) {
                cache[i] = (seq_t)10;
            } else if (idx == 2) {
                /* a(2) = a(0) + 10*a(1) */
                cache[i] = cache[0] + (seq_t)10 * cache[1];
            } else {
                /* a(n) = (10^(n-1)+1) * a(n-1) for n >= 3 */
                seq_t factor = pow10(idx - 1) + (seq_t)1;
                cache[i] = factor * cache[i - 1];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
