/*
 * A004424  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-23).
 * https://oeis.org/A004424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficients of 1/theta(x)^23, theta(x) = 1 +
 *            2*Sum_{k>=1} x^(k^2); computed by dividing the series 1 by theta
 *            23 times in place: c(i) <- c(i) - 2*Sum_{k>=1, k^2<=i} c(i-k^2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 22, i128 n <= 65,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cached_result = NULL;
static idx_t cached_len = 0;

seq_t a(idx_t n) {
    idx_t required_len = n + 1;

    if (cached_result != NULL && cached_len >= required_len) {
        return cached_result[n];
    }

    idx_t len = required_len;
    if (len < 2 * cached_len) len = 2 * cached_len;

    seq_t *c = alloc_zero(len);
    c[0] = 1;
    for (int step = 0; step < 23; step++) {
        for (idx_t i = 1; i < len; i++) {
            seq_t s = 0;
            for (idx_t k = 1; k * k <= i; k++) {
                s += c[i - k * k];
            }
            c[i] -= s * 2;
        }
    }

    if (cached_result != NULL) {
        release(cached_result, cached_len);
    }
    cached_result = c;
    cached_len = len;

    return c[n];
}
