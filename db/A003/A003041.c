/*
 * A003041  Number of vacuously transitive relations on n nodes up to
 *          isomorphism.
 * https://oeis.org/A003041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(n) - 2*B(n-1) + B(n-2), where B(m) = Sum_{s+t=m}
 *            Sum_{nu |- s} (2^len(nu)/z(nu)) * [y^t] Prod_{L>=1}
 *            (1-y^L)^(-e_L), with Sum_{d|k} d*e_d = 2^(1+Sum_{i in nu}
 *            gcd(i,k)) (Burnside count of bipartite graphs with 2-coloured
 *            vertices); the relations are exactly loops arbitrary + non-loop
 *            edg...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 18, u128 n <= 23,
 *            big any n
 * Helpers:   vt_gcd, vt_mul, vt_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/vt_gcd.h"
#include "../utils/vt_mul.h"
#include "../utils/vt_pow.h"

#define VT_MAXN 400
#define VT_NP 3

static const uint64_t vt_p[VT_NP] = {2305843009213693951ULL, 1000000000000000003ULL, 1000000000000000009ULL};

static int vt_N;
static int vt_big = -1;
static int vt_have = -1;
static int vt_parts[VT_MAXN + 1];
static int vt_w[VT_MAXN + 1];
static seq_t vt_rowB[VT_MAXN + 1], vt_PB[VT_MAXN + 1], vt_totB[VT_MAXN + 1];
static seq_t vt_fact;
static uint64_t vt_rowM[VT_NP][VT_MAXN + 1], vt_PM[VT_MAXN + 1];
static uint64_t vt_totM[VT_NP][VT_MAXN + 1], vt_inv[VT_NP][VT_MAXN + 1];

static void vt_leaf(int s, int l) {
    int T = vt_N - s, k, t, i, j;
    for (k = 1; k <= T; k++) {
        int w = 0;
        for (i = 0; i < l; i++) w += vt_gcd(vt_parts[i], k);
        vt_w[k] = w;
    }
    if (vt_big) {
        seq_t z = 1, wt;
        int run = 0;
        for (i = 0; i < l; i++) {
            if (i > 0 && vt_parts[i] == vt_parts[i - 1]) run++; else run = 1;
            z *= (idx_t)vt_parts[i];
            z *= (idx_t)run;
        }
        wt = (vt_fact / z) << (idx_t)l;
        vt_PB[0] = 1;
        for (t = 1; t <= T; t++) {
            seq_t acc = 0;
            for (k = 1; k <= t; k++) acc += vt_PB[t - k] << (idx_t)(vt_w[k] + 1);
            vt_PB[t] = acc / (idx_t)t;
        }
        for (t = s; t <= T; t++) vt_rowB[t] += wt * vt_PB[t];
    } else {
        for (j = 0; j < VT_NP; j++) {
            uint64_t p = vt_p[j], z = 1, wt;
            uint64_t c[VT_MAXN + 1];
            int run = 0;
            for (i = 0; i < l; i++) {
                if (i > 0 && vt_parts[i] == vt_parts[i - 1]) run++; else run = 1;
                z = vt_mul(z, (uint64_t)vt_parts[i], p);
                z = vt_mul(z, (uint64_t)run, p);
            }
            wt = vt_mul(vt_pow(2, (uint64_t)l, p), vt_pow(z, p - 2, p), p);
            for (k = 1; k <= T; k++) c[k] = vt_pow(2, (uint64_t)vt_w[k] + 1, p);
            vt_PM[0] = 1;
            for (t = 1; t <= T; t++) {
                uint64_t acc = 0;
                for (k = 1; k <= t; k++) {
                    acc += vt_mul(c[k], vt_PM[t - k], p);
                    if (acc >= p) acc -= p;
                }
                vt_PM[t] = vt_mul(acc, vt_inv[j][t], p);
            }
            for (t = s; t <= T; t++)
                vt_rowM[j][t] = (vt_rowM[j][t] + vt_mul(wt, vt_PM[t], p)) % p;
        }
    }
}

static void vt_rec(int rem, int maxp, int len, int s) {
    int q;
    if (rem == 0) { vt_leaf(s, len); return; }
    for (q = (rem < maxp ? rem : maxp); q >= 1; q--) {
        vt_parts[len] = q;
        vt_rec(rem - q, q, len + 1, s);
    }
}

static void vt_build(int N) {
    int s, t, j;
    vt_N = N;
    if (vt_big) {
        for (t = 0; t <= N; t++) vt_totB[t] = 0;
        vt_fact = 1;
    } else {
        for (j = 0; j < VT_NP; j++) {
            for (t = 0; t <= N; t++) vt_totM[j][t] = 0;
            for (t = 1; t <= N; t++) vt_inv[j][t] = vt_pow((uint64_t)t, vt_p[j] - 2, vt_p[j]);
        }
    }
    for (s = 0; 2 * s <= N; s++) {
        if (vt_big) {
            if (s > 0) vt_fact *= (idx_t)s;
            for (t = 0; t <= N; t++) vt_rowB[t] = 0;
        } else {
            for (j = 0; j < VT_NP; j++)
                for (t = 0; t <= N; t++) vt_rowM[j][t] = 0;
        }
        vt_rec(s, s, 0, s);
        for (t = s; s + t <= N; t++) {
            int mult = (s < t) ? 2 : 1;
            if (vt_big) {
                seq_t b = vt_rowB[t] / vt_fact;
                vt_totB[s + t] += b * (idx_t)mult;
            } else {
                for (j = 0; j < VT_NP; j++)
                    vt_totM[j][s + t] = (vt_totM[j][s + t] + vt_mul(vt_rowM[j][t], (uint64_t)mult, vt_p[j])) % vt_p[j];
            }
        }
    }
    vt_have = N;
}

seq_t a(idx_t n_idx) {
    int n, j;
    if (vt_big < 0) {
        seq_t x = 1;
        for (j = 0; j < 130; j++) x *= 2;
        vt_big = (x != 0) ? 1 : 0;
    }
    if (n_idx > VT_MAXN) return 0;
    n = (int)n_idx;
    if (n > vt_have) vt_build(n);
    if (vt_big) {
        seq_t r = vt_totB[n];
        if (n >= 2) r += vt_totB[n - 2];
        if (n >= 1) r -= vt_totB[n - 1] * 2;
        return r;
    } else {
        uint64_t r[VT_NP];
        uint64_t p1 = vt_p[0], p2 = vt_p[1], p3 = vt_p[2], k2, k3, tt;
        unsigned __int128 x;
        seq_t res;
        int sh = 32;
        for (j = 0; j < VT_NP; j++) {
            uint64_t p = vt_p[j];
            uint64_t v = vt_totM[j][n];
            if (n >= 2) v += vt_totM[j][n - 2];
            if (n >= 1) v += 2 * (p - vt_totM[j][n - 1]);
            r[j] = v % p;
        }
        k2 = vt_mul((r[1] + p2 - r[0] % p2) % p2, vt_pow(p1 % p2, p2 - 2, p2), p2);
        tt = (r[2] + p3 - r[0] % p3) % p3;
        tt = (tt + p3 - vt_mul(p1 % p3, k2 % p3, p3)) % p3;
        k3 = vt_mul(tt, vt_pow(vt_mul(p1 % p3, p2 % p3, p3), p3 - 2, p3), p3);
        x = (unsigned __int128)r[0] + (unsigned __int128)p1 * k2
            + (unsigned __int128)p1 * p2 * k3;
        res = (seq_t)(uint64_t)x;
        if (sizeof(seq_t) > 8)
            res += ((seq_t)(uint64_t)(x >> 64) << sh) << sh;
        return res;
    }
}
