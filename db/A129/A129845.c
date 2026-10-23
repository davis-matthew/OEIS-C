/*
 * A129845  Numbers n such that n and 2n share at least one digit.
 * https://oeis.org/A129845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129845: a(n) = nth positive integer such that n and 2n share at least one digit. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

static bool shares_digit(seq_t a, seq_t b)
{
    unsigned char cnt[10];
    digit_counts(a, cnt);
    unsigned int mask_a = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i]) mask_a |= 1u << i;

    digit_counts(b, cnt);
    unsigned int mask_b = 0;
    for (int i = 0; i < 10; ++i)
        if (cnt[i]) mask_b |= 1u << i;

    return (mask_a & mask_b) != 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;

    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    if (n <= cache_len)
        return cache[n - 1];

    for (seq_t cur = last_checked + 1; ; ++cur) {
        seq_t twice = cur * ((seq_t)2);
        if (shares_digit(cur, twice)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur;
            if (cache_len >= n) {
                last_checked = cur;
                break;
            }
        }
        last_checked = cur;
    }

    return cache[n - 1];
}
