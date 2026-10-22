/*
 * A004422  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-21).
 * https://oeis.org/A004422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] theta_3(x)^(-21). For fixed-width seq_t: a(n) =
 *            (-1)^n * [x^n] P(x)^21, where P(x) = 1/theta_3(-x) =
 *            Product_{k>=1} (1+x^k)/(1-x^k), the overpartition g.f.
 *            (A015128). For bigint seq_t, the J.C.P. Miller power recurrence:
 *            n*a(n) = -2 * Sum_{j>=1, j^2<=n} (20*j^2 + n) * a(n-j^2), a(0) =
 *            1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 23, i128 n <= 69,
 *            big any n
 * Helpers:   a004422_mul, release, seq_is_fixed_width, zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/zalloc_s.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a004422_mul.h"

seq_t a(idx_t n) {
    seq_t ans;
    if (seq_is_fixed_width()) {
        /* All intermediate coefficients are positive and bounded by the final |a(n)|. */
        seq_t *p = zalloc_s(n + 1);
        seq_t *res = zalloc_s(n + 1);
        seq_t *tmp = zalloc_s(n + 1);
        seq_t *t;
        idx_t k, i;
        unsigned e = 21;
        p[0] = 1;
        for (k = 1; k <= n; k++) {
            for (i = k; i <= n; i++) p[i] += p[i - k];
            for (i = n + 1; i-- > k;) p[i] += p[i - k];
        }
        res[0] = 1;
        while (e) {
            if (e & 1) {
                a004422_mul(res, p, tmp, n);
                t = res; res = tmp; tmp = t;
            }
            e >>= 1;
            if (e) {
                a004422_mul(p, p, tmp, n);
                t = p; p = tmp; tmp = t;
            }
        }
        ans = res[n];
        if (n & 1) ans = -ans;
        release(p, n + 1);
        release(res, n + 1);
        release(tmp, n + 1);
    } else {
        seq_t *g = zalloc_s(n + 1);
        idx_t m, j;
        g[0] = 1;
        for (m = 1; m <= n; m++) {
            seq_t s = 0;
            for (j = 1; j * j <= m; j++) {
                s += ((seq_t)20 * (seq_t)(j * j) + (seq_t)m) * g[m - j * j];
            }
            g[m] = (seq_t)(-2) * s / (seq_t)m;
        }
        ans = g[n];
        release(g, n + 1);
    }
    return ans;
}
