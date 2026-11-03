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
 * Validated: standard, pass, 34 terms through n = 8880 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
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
