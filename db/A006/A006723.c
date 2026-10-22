/*
 * A006723  Somos-7 sequence: a(n) = (a(n-1) * a(n-6) + a(n-2) * a(n-5) +
 *          a(n-3) * a(n-4)) / a(n-7), a(0) = ... = a(6) = 1.
 * https://oeis.org/A006723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (a(n-1)*a(n-6) + a(n-2)*a(n-5) + a(n-3)*a(n-4)) / a(n-7),
 *            a(0) = ... = a(6) = 1. All terms are odd, so for fixed-width
 *            seq_t the division is done as multiplication by the inverse of
 *            a(n-7) modulo 2^w, which is exact whenever a(n) fits
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 21, u64 n <= 29, u128 n <= 40,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed = 0;

static seq_t mulw(seq_t x, seq_t y) {
    return (seq_t)(1u * x * y);
}

static seq_t inv_mod_2w(seq_t v) {
    /* Newton iteration for the inverse of odd v modulo 2^w: 3 -> 6 -> ... -> 192 bits */
    seq_t x = v;
    int i;
    for (i = 0; i < 7; i++) {
        x = mulw(x, (seq_t)((seq_t)2 - mulw(v, x)));
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t i;
    bool fixed;
    if (n <= 6) return 1;
    if (n >= cache_len) {
        idx_t newlen = cache_len ? cache_len : 64;
        seq_t *nc;
        while (newlen <= n) newlen *= 2;
        nc = (seq_t *)malloc(newlen * sizeof(seq_t));
        for (i = 0; i < computed; i++) nc[i] = cache[i];
        free(cache);
        cache = nc;
        cache_len = newlen;
    }
    if (computed == 0) {
        for (i = 0; i <= 6; i++) cache[i] = 1;
        computed = 7;
    }
    fixed = seq_is_fixed_width();
    for (i = computed; i <= n; i++) {
        if (fixed) {
            seq_t num = (seq_t)(mulw(cache[i-1], cache[i-6]) + mulw(cache[i-2], cache[i-5]));
            num = (seq_t)(num + mulw(cache[i-3], cache[i-4]));
            cache[i] = mulw(num, inv_mod_2w(cache[i-7]));
        } else {
            seq_t num = cache[i-1] * cache[i-6] + cache[i-2] * cache[i-5] + cache[i-3] * cache[i-4];
            cache[i] = num / cache[i-7];
        }
    }
    if (n + 1 > computed) computed = n + 1;
    return cache[n];
}
