/*
 * A005375  a(0) = 0; a(n) = n - a(a(a(a(n-1)))) for n > 0.
 * https://oeis.org/A005375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n - a(a(a(a(n-1)))), a(0) = 0
 * Range:     u8 n <= 352, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t highest_computed = 0;

    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 0;
        highest_computed = 0;
    }

    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (n >= new_len) new_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        for (idx_t i = 0; i <= highest_computed; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    if (n > highest_computed) {
        for (idx_t i = highest_computed + 1; i <= n; i++) {
            seq_t v = cache[i - 1];               /* a(n-1) */
            v = cache[(idx_t)(long long)v];       /* a(a(n-1)) */
            v = cache[(idx_t)(long long)v];       /* a(a(a(n-1))) */
            v = cache[(idx_t)(long long)v];       /* a(a(a(a(n-1)))) */
            cache[i] = (seq_t)i - v;
        }
        highest_computed = n;
    }

    return cache[n];
}
