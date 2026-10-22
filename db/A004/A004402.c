/*
 * A004402  Expansion of 1 / Sum_{n=-oo..oo} x^(n^2).
 * https://oeis.org/A004402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(q), theta_3 = 1 + 2*Sum_{k>=1} q^(k^2); so a(0) =
 *            1, a(n) = -2*Sum_{k>=1, k^2<=n} a(n-k^2)
 * Range:     i8 n <= 8, i16 n <= 25, i32 n <= 79, i64 n <= 267, i128 n <= 952,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len) {
        return cache[n];
    }

    idx_t new_len = n + 1;
    cache = grow_zero(cache, cache_len, new_len);

    idx_t start = cache_len;
    if (start == 0) {
        cache[0] = 1;
        start = 1;
    }

    for (idx_t i = start; i < new_len; i++) {
        seq_t sum = 0;
        for (idx_t k = 1; k * k <= i; k++) {
            sum += cache[i - k * k];
        }
        cache[i] = sum * (-2);
    }

    cache_len = new_len;
    return cache[n];
}
