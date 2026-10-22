/*
 * A007566  a(n+1) = (2n+3)*a(n) - 2n*a(n-1) + 8n, a(0) = 1, a(1) = 3.
 * https://oeis.org/A007566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*a(n-1) + (2*n-1)^2, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;
    }

    if (n < cache_len) {
        return cache[n];
    }

    seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i < cache_len; i++) {
        new_cache[i] = cache[i];
    }
    free(cache);
    cache = new_cache;

    for (idx_t i = cache_len; i <= n; i++) {
        seq_t two_i = (seq_t)(2 * i);
        seq_t two_i_minus_1 = (seq_t)(2 * i - 1);
        cache[i] = two_i * cache[i - 1] + two_i_minus_1 * two_i_minus_1;
    }

    cache_len = n + 1;
    return cache[n];
}
