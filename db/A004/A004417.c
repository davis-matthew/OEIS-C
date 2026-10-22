/*
 * A004417  Expansion of (Sum x^(n^2), n = -inf .. inf )^(-16).
 * https://oeis.org/A004417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^16 with theta_3(x) = 1 + 2*Sum_{m>=1} x^(m^2).
 *            Arbitrary precision: J.C.P. Miller power recurrence a(0) = 1,
 *            a(n) = (1/n) * Sum_{m>=1, m^2<=n} 2*(-15*m^2 - n)*a(n-m^2).
 *            Fixed width (division-free, wrap-safe): h = 1/theta_3, h(0) = 1,
 *            h(n) = -2*Sum_{m>=1, m^2<=n} h(n-m^2); then a...
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 27, i128 n <= 84,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *F = NULL;
static idx_t flen = 0;

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        seq_t *p = alloc_zero(n + 1);
        seq_t *q = alloc_zero(n + 1);
        seq_t *t;
        seq_t result;
        idx_t i, j, m;
        int r;
        p[0] = 1;
        for (i = 1; i <= n; i++) {
            seq_t s = 0;
            for (m = 1; m * m <= i; m++)
                s += p[i - m * m];
            p[i] = s * (-2);
        }
        for (r = 0; r < 4; r++) {
            for (i = 0; i <= n; i++) {
                seq_t s = 0;
                for (j = 0; j <= i; j++)
                    s += p[j] * p[i - j];
                q[i] = s;
            }
            t = p; p = q; q = t;
        }
        result = p[n];
        release(p, n + 1);
        release(q, n + 1);
        return result;
    }

    if (flen == 0) {
        F = alloc_zero(1);
        F[0] = 1;
        flen = 1;
    }
    if (n >= flen) {
        idx_t i, m;
        F = grow_zero(F, flen, n + 1);
        for (i = flen; i <= n; i++) {
            seq_t s = 0;
            for (m = 1; m * m <= i; m++) {
                seq_t c = (seq_t)0 - (seq_t)(15 * m * m + i);
                s += c * 2 * F[i - m * m];
            }
            F[i] = s / (seq_t)i;
        }
        flen = n + 1;
    }
    return F[n];
}
