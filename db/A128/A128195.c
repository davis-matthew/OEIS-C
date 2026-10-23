/*
 * A128195  a(n) = (2*n + 1)*(a(n - 1) + 2^n) for n >= 1, a(0) = 1.
 * https://oeis.org/A128195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 1)*(a(n - 1) + 2^n); a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static seq_t pow2_current = 1;

    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache_size = 1;
        pow2_current = 1;
    }

    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            pow2_current *= (seq_t)2;
            cache[i] = ((seq_t)(2 * i + 1)) * (cache[i - 1] + pow2_current);
        }
        cache_size = new_size;
    }

    return cache[n];
}
