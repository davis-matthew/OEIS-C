/*
 * A004416  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-15).
 * https://oeis.org/A004416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^15. For arbitrary precision: n*a(n) =
 *            -Sum_{k>=1, k^2<=n} 2*(n + 14*k^2)*a(n-k^2), a(0) = 1. For fixed
 *            width: g = 1/theta_3 via g(n) = -2*Sum_{k>=1} g(n-k^2), g(0) =
 *            1, then a = g^15 by truncated series multiplication (no
 *            cancellation, so no overflow)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 28, i128 n <= 88,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 9260 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static void mulser(seq_t *r, const seq_t *x, const seq_t *y, idx_t len) {
    for (idx_t i = 0; i < len; i++) {
        seq_t s = 0;
        for (idx_t j = 0; j <= i; j++)
            s += x[j] * y[i - j];
        r[i] = s;
    }
}

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t ans;
    if (!seq_is_fixed_width()) {
        seq_t *f = alloc_zero(len);
        f[0] = 1;
        for (idx_t i = 1; i <= n; i++) {
            seq_t s = 0;
            for (idx_t k = 1; k * k <= i; k++) {
                seq_t c = (seq_t)i + (seq_t)(14 * k * k);
                s += c * f[i - k * k];
            }
            f[i] = -(s * 2) / (seq_t)i;
        }
        ans = f[n];
        release(f, len);
        return ans;
    }
    {
        seq_t *g = alloc_zero(len);
        seq_t *acc = alloc_zero(len);
        seq_t *sq = alloc_zero(len);
        seq_t *tmp = alloc_zero(len);
        g[0] = 1;
        for (idx_t i = 1; i <= n; i++) {
            seq_t s = 0;
            for (idx_t k = 1; k * k <= i; k++)
                s += g[i - k * k];
            g[i] = -(s * 2);
        }
        for (idx_t i = 0; i < len; i++) {
            acc[i] = g[i];
            sq[i] = g[i];
        }
        for (int step = 0; step < 3; step++) {
            mulser(tmp, sq, sq, len);
            for (idx_t i = 0; i < len; i++) sq[i] = tmp[i];
            mulser(tmp, acc, sq, len);
            for (idx_t i = 0; i < len; i++) acc[i] = tmp[i];
        }
        ans = acc[n];
        release(g, len);
        release(acc, len);
        release(sq, len);
        release(tmp, len);
    }
    return ans;
}
