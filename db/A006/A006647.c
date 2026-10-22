/*
 * A006647  Number of graphs with n nodes, n-2 edges and no isolated vertices.
 * https://oeis.org/A006647
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001430(n) - A001433(n-1), i.e. (graphs on n nodes with
 *            n-2 edges) - (graphs on n-1 nodes with n-2 edges). Each count is
 *            the x^(n-2) coefficient of Polya's cycle index of S_N acting on
 *            pairs, evaluated at 1+x and summed over partitions of N. The
 *            arithmetic is done modulo several 61-bit pri...
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   a6647_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/a6647_gcd.h"

typedef unsigned __int128 a6647_u128;

static uint64_t a6647_mulm(uint64_t a, uint64_t b, uint64_t p) { return (uint64_t)((a6647_u128)a * b % p); }
static uint64_t a6647_powm(uint64_t a, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p; a %= p;
    while (e) { if (e & 1) r = a6647_mulm(r, a, p); a = a6647_mulm(a, a, p); e >>= 1; }
    return r;
}
static bool a6647_mr(uint64_t n) {
    static const uint64_t bs[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i, s = 0, r;
    uint64_t d;
    if (n < 2) return false;
    for (i = 0; i < 12; i++) { if (n % bs[i] == 0) return n == bs[i]; }
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 12; i++) {
        uint64_t x = a6647_powm(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        for (r = 1; r < s; r++) { x = a6647_mulm(x, x, n); if (x == n - 1) break; }
        if (r >= s) return false;
    }
    return true;
}

static uint64_t a6647_pr[1024];
static int a6647_npr = 0;
static uint64_t a6647_cand = (((uint64_t)1) << 61) - 1;
static uint64_t a6647_prime(int i) {
    while (a6647_npr <= i) {
        while (!a6647_mr(a6647_cand)) a6647_cand -= 2;
        a6647_pr[a6647_npr++] = a6647_cand;
        a6647_cand -= 2;
    }
    return a6647_pr[i];
}

/* run state */
static int gK, gM, gN1;
static const uint64_t *gP;
static uint64_t *gPoly, *gWt, *gInv, *gBc, *gAcc;
static int *gPk, *gPc;

#define A6647_POLY(lv, i) (gPoly + ((size_t)(lv) * gK + (i)) * (size_t)(gM + 1))

static void a6647_apply(int lv, uint64_t L, uint64_t E) {
    int i;
    uint64_t tmax, t, d;
    if (E == 0 || L > (uint64_t)gM) return;
    tmax = (uint64_t)gM / L;
    if (E < tmax) tmax = E;
    for (i = 0; i < gK; i++) {
        uint64_t p = gP[i];
        uint64_t *poly = A6647_POLY(lv, i);
        const uint64_t *inv = gInv + (size_t)i * gN1;
        gBc[0] = 1;
        for (t = 1; t <= tmax; t++)
            gBc[t] = a6647_mulm(a6647_mulm(gBc[t - 1], (E - t + 1) % p, p), inv[t], p);
        for (d = (uint64_t)gM; d >= L; d--) {
            uint64_t s = poly[d];
            for (t = 1; t <= tmax && t * L <= d; t++) {
                s += a6647_mulm(gBc[t], poly[d - t * L], p);
                if (s >= p) s -= p;
            }
            poly[d] = s;
        }
    }
}

static void a6647_dfs(int d, int prev, int rem) {
    int k, c, j, i, t;
    size_t lvsz = (size_t)gK * (size_t)(gM + 1);
    for (k = prev + 1; k <= rem; k++) {
        for (c = 1; c * k <= rem; c++) {
            int r = rem - c * k;
            if (r != 0 && r <= k) continue;
            memcpy(gPoly + (size_t)(d + 1) * lvsz, gPoly + (size_t)d * lvsz, lvsz * sizeof(uint64_t));
            for (j = 0; j < d; j++) {
                int g = a6647_gcd(gPk[j], k);
                uint64_t L = (uint64_t)(gPk[j] / g) * (uint64_t)k;
                uint64_t E = (uint64_t)g * (uint64_t)gPc[j] * (uint64_t)c;
                a6647_apply(d + 1, L, E);
            }
            a6647_apply(d + 1, (uint64_t)k,
                        (uint64_t)k * (uint64_t)c * (uint64_t)(c - 1) / 2 + (uint64_t)c * (uint64_t)((k - 1) / 2));
            if (k % 2 == 0) a6647_apply(d + 1, (uint64_t)(k / 2), (uint64_t)c);
            for (i = 0; i < gK; i++) {
                uint64_t p = gP[i];
                const uint64_t *inv = gInv + (size_t)i * gN1;
                uint64_t w = a6647_mulm(gWt[(size_t)d * gK + i], a6647_powm(inv[k], (uint64_t)c, p), p);
                for (t = 1; t <= c; t++) w = a6647_mulm(w, inv[t], p);
                if (r == 0) {
                    gAcc[i] = (gAcc[i] + a6647_mulm(w, A6647_POLY(d + 1, i)[gM], p)) % p;
                } else {
                    gWt[(size_t)(d + 1) * gK + i] = w;
                }
            }
            if (r != 0) {
                gPk[d] = k; gPc[d] = c;
                a6647_dfs(d + 1, k, r);
            }
        }
    }
}

/* residues of number of graphs with N nodes and M edges */
static void a6647_count(int N, int M, const uint64_t *ps, int K, uint64_t *out) {
    int levels = 3, i, t;
    while ((levels - 3) * (levels - 3) <= 2 * N) levels++;
    levels += 2;
    gK = K; gM = M; gP = ps; gN1 = N + 2;
    gPoly = (uint64_t *)calloc((size_t)levels * K * (size_t)(M + 1), sizeof(uint64_t));
    gWt = (uint64_t *)calloc((size_t)levels * K, sizeof(uint64_t));
    gInv = (uint64_t *)calloc((size_t)K * gN1, sizeof(uint64_t));
    gBc = (uint64_t *)calloc((size_t)M + 2, sizeof(uint64_t));
    gPk = (int *)calloc((size_t)levels, sizeof(int));
    gPc = (int *)calloc((size_t)levels, sizeof(int));
    gAcc = out;
    for (i = 0; i < K; i++) {
        uint64_t p = ps[i];
        uint64_t *inv = gInv + (size_t)i * gN1;
        inv[1] = 1;
        for (t = 2; t < gN1; t++)
            inv[t] = (p - a6647_mulm(p / (uint64_t)t, inv[p % (uint64_t)t], p)) % p;
        A6647_POLY(0, i)[0] = 1;
        gWt[i] = 1;
        out[i] = 0;
    }
    if (N == 0) { for (i = 0; i < K; i++) out[i] = (M == 0) ? 1 : 0; }
    else a6647_dfs(0, 0, N);
    free(gPoly); free(gWt); free(gInv); free(gBc); free(gPk); free(gPc);
}

static void a6647_res(int n, const uint64_t *ps, int K, uint64_t *out) {
    uint64_t *r2 = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)K);
    int i;
    a6647_count(n, n - 2, ps, K, out);
    a6647_count(n - 1, n - 2, ps, K, r2);
    for (i = 0; i < K; i++) out[i] = (out[i] + ps[i] - r2[i]) % ps[i];
    free(r2);
}

seq_t a(idx_t nn) {
    int n = (int)nn;
    int K = (2 * n + 64) / 60 + 2, have = 0, i, j;
    uint64_t *ps, *rs, *v;
    seq_t val;
    if (K > 1000) K = 1000;
    ps = (uint64_t *)malloc(sizeof(uint64_t) * 1024);
    rs = (uint64_t *)malloc(sizeof(uint64_t) * 1024);
    v = (uint64_t *)malloc(sizeof(uint64_t) * 1024);
    for (;;) {
        for (i = have; i < K; i++) ps[i] = a6647_prime(i);
        a6647_res(n, ps + have, K - have, rs + have);
        have = K;
        for (i = 0; i < K; i++) {
            uint64_t p = ps[i], x = rs[i] % p;
            for (j = 0; j < i; j++) {
                uint64_t vj = v[j] % p;
                x = (x + p - vj) % p;
                x = a6647_mulm(x, a6647_powm(ps[j] % p, p - 2, p), p);
            }
            v[i] = x;
        }
        if (v[K - 1] == 0 || K >= 1000) break;
        K++;
    }
    val = 0;
    for (i = K - 2; i >= 0; i--) val = val * (seq_t)ps[i] + (seq_t)v[i];
    free(ps); free(rs); free(v);
    return val;
}
