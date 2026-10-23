/*
 * A022097  Fibonacci sequence beginning 1, 7.
 * https://oeis.org/A022097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure the first two terms are available */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
        cache[0] = 1;
        cache[1] = 7;
    } else if (cache_len == 1) {  /* Just in case */
        cache[1] = 7;
    }

    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Extend the cache up to index n */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
