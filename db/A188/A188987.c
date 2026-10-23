/*
 * A188987  Number of n X 4 binary arrays without the pattern 0 0 1
 *          antidiagonally or horizontally.
 * https://oeis.org/A188987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-3) + 24*a(n-4) + 36*a(n-5) - 48*a(n-6)
 *            + 16*a(n-7) Initial values (for n = 1 .. 7): {12, 144, 1256,
 *            10812, 92532, 791388, 6768028}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
static seq_t *cache = nullptr;
static size_t cap = 0, len = 0;
static void init_cache(void) {
    if (!cache) {
        cap = 8;
        cache = new seq_t[cap];
        cache[0] = (seq_t)12;
        cache[1] = (seq_t)144;
        cache[2] = (seq_t)1256;
        cache[3] = (seq_t)10812;
        cache[4] = (seq_t)92532;
        cache[5] = (seq_t)791388;
        cache[6] = (seq_t)6768028;
        len = 7;
    }
}
static void ensure_cache(idx_t n) {
    init_cache();
    idx_t target = n - 1;
    while ((idx_t)len <= target) {
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = new seq_t[new_cap];
            for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
            delete[] cache;
            cache = new_cache;
            cap = new_cap;
        }
        idx_t i = len;
        seq_t val = cache[i - 1] * (seq_t)9
                  - cache[i - 3] * (seq_t)36
                  + cache[i - 4] * (seq_t)24
                  + cache[i - 5] * (seq_t)36
                  - cache[i - 6] * (seq_t)48
                  + cache[i - 7] * (seq_t)16;
        cache[i] = val;
        ++len;
    }
}
#else
static seq_t *cache = NULL;
static size_t cap = 0, len = 0;
static void init_cache(void) {
    if (!cache) {
        cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)12;
        cache[1] = (seq_t)144;
        cache[2] = (seq_t)1256;
        cache[3] = (seq_t)10812;
        cache[4] = (seq_t)92532;
        cache[5] = (seq_t)791388;
        cache[6] = (seq_t)6768028;
        len = 7;
    }
}
static void ensure_cache(idx_t n) {
    init_cache();
    idx_t target = n - 1;
    while ((idx_t)len <= target) {
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = new_cache;
            cap = new_cap;
        }
        idx_t i = len;
        seq_t val = cache[i - 1] * (seq_t)9
                  - cache[i - 3] * (seq_t)36
                  + cache[i - 4] * (seq_t)24
                  + cache[i - 5] * (seq_t)36
                  - cache[i - 6] * (seq_t)48
                  + cache[i - 7] * (seq_t)16;
        cache[i] = val;
        ++len;
    }
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1, not used */
    ensure_cache(n);
    return cache[n - 1];
}
