/*
 * A000550  Number of trees of diameter 7.
 * https://oeis.org/A000550
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: a(x) = (r(x)^2 + r(x^2))/2, where r(x) = Sum A000235(n)
 *            x^n counts rooted trees of height exactly 3: r(n) = T3(n) -
 *            T2(n), T2(n) = p(n-1) (A000041), T3(n) = [x^(n-1)] Prod_{k>=1}
 *            1/(1-x^k)^T2(k) (Euler transform). So a(n) = Sum_{k<n-k} r(k)
 *            r(n-k) + [n even] r(n/2)(r(n/2)+1)/2
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 37, u64 n <= 80, u128 n <= 182,
 *            big any n
 * Helpers:   a550_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a550_is_big.h"

#define A550_MAX 12000

typedef unsigned __int128 a550_u128;

/* ---------- fixed-width path: exact u128 while values fit ---------- */

static a550_u128 a550_fp[A550_MAX + 1];
static a550_u128 a550_fc[A550_MAX + 1];
static a550_u128 a550_fb[A550_MAX + 1];
static a550_u128 a550_fR[A550_MAX + 1];
static long long a550_fn = 0;

static void a550_extend_fixed(long long n) {
    long long i, k, d;
    for (i = a550_fn; i <= n; i++) {
        if (i == 0) {
            a550_fp[0] = 1;
        } else {
            a550_u128 s = 0;
            for (k = 1;; k++) {
                long long g1 = k * (3 * k - 1) / 2, g2;
                a550_u128 t;
                if (g1 > i) break;
                t = a550_fp[i - g1];
                g2 = g1 + k;
                if (g2 <= i) t += a550_fp[i - g2];
                if (k & 1) s += t; else s -= t;
            }
            a550_fp[i] = s;
        }
        a550_fc[i] = 0;
        if (i >= 1) {
            for (d = 1; d * d <= i; d++) {
                if (i % d == 0) {
                    long long e = i / d;
                    a550_fc[i] += a550_fp[d - 1] * (a550_u128)d;
                    if (e != d) a550_fc[i] += a550_fp[e - 1] * (a550_u128)e;
                }
            }
        }
        if (i == 0) {
            a550_fb[0] = 1;
        } else {
            a550_u128 s = 0;
            for (k = 1; k <= i; k++) s += a550_fc[k] * a550_fb[i - k];
            a550_fb[i] = s / (a550_u128)i;
        }
        if (i < 2) a550_fR[i] = 0;
        else a550_fR[i] = a550_fb[i - 1] - a550_fp[i - 1];
    }
    if (n + 1 > a550_fn) a550_fn = n + 1;
}

static seq_t a550_fixed(idx_t n) {
    long long nn = (long long)n, k;
    a550_u128 v = 0;
    seq_t res = 0;
    int bit;
    if (nn > A550_MAX) nn = A550_MAX;
    if (nn >= a550_fn) a550_extend_fixed(nn);
    for (k = 2; 2 * k < nn; k++) v += a550_fR[k] * a550_fR[nn - k];
    if (nn % 2 == 0) {
        a550_u128 h = a550_fR[nn / 2];
        if (h % 2 == 0) v += (h / 2) * (h + 1);
        else v += h * ((h + 1) / 2);
    }
    for (bit = 127; bit >= 0; bit--) {
        res += res;
        if ((v >> bit) & 1) res += 1;
    }
    return res;
}

/* ---------- arbitrary-precision path ---------- */

static seq_t a550_bp[A550_MAX + 1];
static seq_t a550_bc[A550_MAX + 1];
static seq_t a550_bb[A550_MAX + 1];
static seq_t a550_bR[A550_MAX + 1];
static long long a550_bn = 0;

static void a550_extend_big(long long n) {
    long long i, k, d;
    for (i = a550_bn; i <= n; i++) {
        if (i == 0) {
            a550_bp[0] = 1;
        } else {
            seq_t pos = 0, neg = 0;
            for (k = 1;; k++) {
                long long g1 = k * (3 * k - 1) / 2, g2;
                if (g1 > i) break;
                g2 = g1 + k;
                if (k & 1) {
                    pos += a550_bp[i - g1];
                    if (g2 <= i) pos += a550_bp[i - g2];
                } else {
                    neg += a550_bp[i - g1];
                    if (g2 <= i) neg += a550_bp[i - g2];
                }
            }
            a550_bp[i] = pos - neg;
        }
        a550_bc[i] = 0;
        if (i >= 1) {
            for (d = 1; d * d <= i; d++) {
                if (i % d == 0) {
                    long long e = i / d;
                    a550_bc[i] += a550_bp[d - 1] * (idx_t)d;
                    if (e != d) a550_bc[i] += a550_bp[e - 1] * (idx_t)e;
                }
            }
        }
        if (i == 0) {
            a550_bb[0] = 1;
        } else {
            seq_t s = 0;
            for (k = 1; k <= i; k++) s += a550_bc[k] * a550_bb[i - k];
            a550_bb[i] = s / (idx_t)i;
        }
        if (i < 2) a550_bR[i] = 0;
        else a550_bR[i] = a550_bb[i - 1] - a550_bp[i - 1];
    }
    if (n + 1 > a550_bn) a550_bn = n + 1;
}

static seq_t a550_big(idx_t n) {
    long long nn = (long long)n, k;
    seq_t v = 0;
    if (nn > A550_MAX) nn = A550_MAX;
    if (nn >= a550_bn) a550_extend_big(nn);
    for (k = 2; 2 * k < nn; k++) v += a550_bR[k] * a550_bR[nn - k];
    if (nn % 2 == 0) {
        seq_t h = a550_bR[nn / 2];
        v += (h * h + h) / 2;
    }
    return v;
}

seq_t a(idx_t n) {
    static int mode = -1;
    if (mode < 0) mode = a550_is_big();
    if (mode) return a550_big(n);
    return a550_fixed(n);
}
