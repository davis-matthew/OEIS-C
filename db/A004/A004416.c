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
 * Helpers:   alloc_zero, mulser, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/mulser.h"

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
