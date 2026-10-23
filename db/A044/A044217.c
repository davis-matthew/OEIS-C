/*
 * A044217  Numbers n such that string 3,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑8 representation
 *            contains the substring 36 but the one preceding it does not
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool has36(seq_t v)
{
    if (v == 0) return false;

    uint8_t digits[64];
    size_t len = 0;

    while (v > 0) {
        digits[len++] = (uint8_t)(v % 8);
        v /= 8;
    }
    if (len < 2) return false;

    for (size_t i = len - 1; i > 0; --i) {
        if (digits[i] == 3 && digits[i - 1] == 6)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        ++last_checked;
        if (has36((seq_t)last_checked)) {
            seq_t prev = (seq_t)last_checked - 1;
            if (!has36(prev)) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }

    return cache[n - 1];
}
