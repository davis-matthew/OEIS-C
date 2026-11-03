/*
 * A004415  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-14).
 * https://oeis.org/A004415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. A(x) = 1/theta_3(x)^14, theta_3(x) = Sum_{k=-inf..inf}
 *            x^(k^2). Power-series recurrence (J.C.P. Miller): a(0) = 1,
 *            n*a(n) = Sum_{j>=1, j^2<=n} 2*(-13*j^2 - n)*a(n-j^2).
 *            Fixed-width builds instead use a(n) = -Sum_{k=1..n} t(k)*a(n-k),
 *            t(k) = [x^k] theta_3(x)^14, which needs no division, so wr...
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 29, i128 n <= 92,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 9948 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *r = alloc_zero(len);
    seq_t ans;
    r[0] = 1;

    if (!seq_is_fixed_width()) {
        for (idx_t i = 1; i <= n; i++) {
            seq_t s = 0;
            for (idx_t j = 1; j * j <= i; j++) {
                seq_t c = (seq_t)(13 * j * j + i);
                s -= c * 2 * r[i - j * j];
            }
            r[i] = s / (seq_t)i;
        }
    } else {
        /* theta_3(x)^14 via sparse multiplications */
        seq_t *t = alloc_zero(len);
        t[0] = 1;
        for (int p = 0; p < 14; p++) {
            seq_t *nt = alloc_zero(len);
            for (idx_t i = 0; i <= n; i++) {
                if (t[i] == 0) continue;
                nt[i] += t[i];
                for (idx_t j = 1; i + j * j <= n; j++)
                    nt[i + j * j] += 2 * t[i];
            }
            release(t, len);
            t = nt;
        }
        for (idx_t i = 1; i <= n; i++) {
            seq_t s = 0;
            for (idx_t k = 1; k <= i; k++)
                s += t[k] * r[i - k];
            r[i] = -s;
        }
        release(t, len);
    }

    ans = r[n];
    release(r, len);
    return ans;
}
