/*
 * A051863  Duplicate of A000699.
 * https://oeis.org/A051863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n-1)*Sum_{i=1..n-1} a(i)*a(n-i) for n > 1, with a(0) =
 *            a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t A000699_a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n == 0 || n == 1) return 1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *new_cache = mem_new(seq_t, new_cap);
            for (idx_t j = 0; j < cache_len; j++) new_cache[j] = cache[j];
            mem_free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        idx_t k = cache_len;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; i++) {
            sum = sum + cache[i] * cache[k - i];
        }
        cache[k] = ((seq_t)(k - 1)) * sum;
        cache_len++;
    }

    return cache[n];
}

seq_t a(idx_t n) {
    return A000699_a(n);
}
