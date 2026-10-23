/*
 * A176304  a(n) = (-1)^n * n * a(n-1) - 1, with a(0)=0.
 * https://oeis.org/A176304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * n * a(n-1) - 1, a(0)=0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    if (n >= cache_cap) {
        /* Grow cache to at least n+1 */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = alloc_cache((idx_t)new_cap);
        if (!new_cache) abort();
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Ensure a(0) is set at first use */
    if (cache[0] == 0 && last_computed == 0)
        cache[0] = 0;

    if (n == 0) return cache[0];

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t sign = ((i & 1ULL) ? -1 : 1);
            cache[i] = sign * (seq_t)i * cache[i - 1] - 1;
        }
        last_computed = n;
    }

    return cache[n];
}
