/*
 * A174522  Expansion of 1/(1 - x - x^4 + x^6).
 * https://oeis.org/A174522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-6), with a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=2, a(5)=3, a(6)=3
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Persistent cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of initialized terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure enough capacity for n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap < 8) ? 8 : cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed, abort */
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First time: initialize known values up to index 6 */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)3;
        cache_len = 7;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i <= 6) {
            /* values already set during initialization */
            continue;
        } else {
            cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 6];
        }
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
