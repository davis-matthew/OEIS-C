/*
 * A006722  Somos-6 sequence: a(n) = (a(n-1) * a(n-5) + a(n-2) * a(n-4) +
 *          a(n-3)^2) / a(n-6), a(0) = ... = a(5) = 1.
 * https://oeis.org/A006722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (a(n-1)*a(n-5) + a(n-2)*a(n-4) + a(n-3)^2) / a(n-6), a(0)
 *            = a(1) = a(2) = a(3) = a(4) = a(5) = 1. All terms are odd, so in
 *            fixed-width builds the exact division is done mod 2^w by
 *            multiplying with the modular inverse of a(n-6)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 19, u64 n <= 25, u128 n <= 35,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 0;

static seq_t mulw(seq_t x, seq_t y) {
    return (seq_t)(x * 1u * y);
}

/* inverse of odd d modulo 2^w (Newton iteration) */
static seq_t inv_odd(seq_t d) {
    seq_t x = d;
    int i;
    for (i = 0; i < 8; i++) {
        seq_t t = (seq_t)(2u - mulw(d, x));
        x = mulw(x, t);
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t i;
    bool fixed;
    if (n < 6) {
        return 1;
    }
    if (cache == NULL) {
        cache_size = 64;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        for (i = 0; i < 6; i++) {
            cache[i] = 1;
        }
        max_computed = 5;
    }
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        seq_t *nc;
        while (new_size <= n) {
            new_size *= 2;
        }
        nc = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        for (i = 0; i <= max_computed; i++) {
            nc[i] = cache[i];
        }
        free(cache);
        cache = nc;
        cache_size = new_size;
    }
    fixed = seq_is_fixed_width();
    for (i = max_computed + 1; i <= n; i++) {
        if (fixed) {
            seq_t num = (seq_t)(mulw(cache[i - 1], cache[i - 5]) + mulw(cache[i - 2], cache[i - 4]));
            num = (seq_t)(num + mulw(cache[i - 3], cache[i - 3]));
            cache[i] = mulw(num, inv_odd(cache[i - 6]));
        } else {
            seq_t num = cache[i - 1] * cache[i - 5] + cache[i - 2] * cache[i - 4] + cache[i - 3] * cache[i - 3];
            cache[i] = num / cache[i - 6];
        }
    }
    if (n > max_computed) {
        max_computed = n;
    }
    return cache[n];
}
