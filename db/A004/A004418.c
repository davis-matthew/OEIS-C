/*
 * A004418  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-17).
 * https://oeis.org/A004418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   g_k = theta^(-k) with theta = Sum_{m=-inf..inf} x^(m^2) = 1 +
 *            2*Sum_{m>=1} x^(m^2); g_0 = 1, and g_k(i) = g_{k-1}(i) -
 *            2*Sum_{m>=1, m^2<=i} g_k(i-m^2); a(n) = g_17(n)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 26, i128 n <= 80,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a004418_cache = NULL;
static idx_t a004418_len = 0;

static void a004418_build(idx_t len) {
    seq_t *prev = alloc_zero(len);
    seq_t *cur = alloc_zero(len);
    prev[0] = 1;
    for (int k = 1; k <= 17; k++) {
        for (idx_t i = 0; i < len; i++) {
            cur[i] = prev[i];
            for (idx_t m = 1; m * m <= i; m++) {
                idx_t s = m * m;
                cur[i] -= cur[i - s];
                cur[i] -= cur[i - s];
            }
        }
        seq_t *t = prev;
        prev = cur;
        cur = t;
    }
    release(cur, len);
    if (a004418_cache != NULL) release(a004418_cache, a004418_len);
    a004418_cache = prev;
    a004418_len = len;
}

seq_t a(idx_t n) {
    if (n >= a004418_len) {
        idx_t len = n + 1;
        if (len < 2 * a004418_len) len = 2 * a004418_len;
        if (len < 64) len = 64;
        a004418_build(len);
    }
    return a004418_cache[n];
}
