/*
 * A176288  Hankel transform of A176287.
 * https://oeis.org/A176288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6*a(n-1) - 17*a(n-2) + 24*a(n-3) - 16*a(n-4) for n>=4
 *            Initial values: a(0)=1, a(1)=3, a(2)=15, a(3)=55
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 25, i64 n <= 56, i128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void init_cache(void) {
    if (cache_len > 0)
        return;
    cache_cap = 4;
    cache_len = 4;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = 1;
    cache[1] = 3;
    cache[2] = 15;
    cache[3] = 55;
}

static void ensure_capacity(size_t target) {
    if (target <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < target)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    size_t idx = (size_t)n;
    init_cache();
    if (idx + 1 > cache_len) {
        ensure_capacity(idx + 1);
        for (size_t i = cache_len; i <= idx; ++i) {
            cache[i] = 6 * cache[i-1]
                     - 17 * cache[i-2]
                     + 24 * cache[i-3]
                     - 16 * cache[i-4];
        }
        cache_len = idx + 1;
    }
    return cache[idx];
}
