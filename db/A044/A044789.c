/*
 * A044789  Numbers n such that string 7,6 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A044789:
 * a(n) is the nth integer whose decimal representation contains
 * the substring "76" but the representation of n+1 does not.
 * The function enumerates successive integers, tests the
 * condition, and memoises the results so that subsequent calls
 * are O(1) for already produced terms. */

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool has76(seq_t x) {
    if (x < 76) return false;
    uint8_t digits[64];
    size_t nd = 0;
    seq_t t = x;
    while (t > 0) {
        digits[nd++] = (uint8_t)(t % 10);
        t /= 10;
    }
    for (size_t i = nd; i > 1; --i) {
        if (digits[i - 1] == 7 && digits[i - 2] == 6) return true;
    }
    return false;
}

#ifdef __cplusplus
static std::vector<seq_t> cache;
static seq_t cache_last = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for index 0 */
    while (cache.size() < n) {
        seq_t x = cache_last + 1;
        if (has76(x) && !has76(x + 1))
            cache.push_back(x);
        cache_last = x;
    }
    return cache[n - 1];
}
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t cache_last = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for index 0 */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }
    while (cache_len < n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        seq_t x = cache_last + 1;
        if (has76(x) && !has76(x + 1))
            cache[cache_len++] = x;
        cache_last = x;
    }
    return cache[n - 1];
}
#endif
