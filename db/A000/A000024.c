/*
 * A000024  Number of positive integers <= 2^n of form x^2 + 10 y^2.
 * https://oeis.org/A000024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{1 <= m <= 2^n : m = x^2 + 10*y^2}. A number m is of
 *            this form iff every prime p with (-40/p) = -1 divides m to an
 *            even power and (v_2(m) + v_5(m) + sum of v_p(m) over primes p ==
 *            7,13,23,37 (mod 40)) is even (class group of discriminant -40
 *            has order 2). Count = (sum_{m<=N} g0(m) + sum_{...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   isq, prefc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isq.h"
#include "../utils/prefc.h"

static const int T40[40] = {
    0, 1, 0,-1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0,-1, 0, 1,
    0,-1, 0, 1, 0, 0, 0,-1, 0,-1, 0,-1, 0,-1, 0, 0, 0, 1, 0,-1 };
static const int T5[5] = { 0, 1, -1, -1, 1 };
static const int T8[8] = { 0, 1, 0, 1, 0, -1, 0, -1 };

static uint64_t g_N, g_r;
static uint64_t *g_w;
static size_t *g_id1, *g_id2;
static uint64_t *g_pr;
static int *g_type;
static size_t g_np;
static int64_t *g_G[2];
static int64_t *g_Gp[2];

static size_t idx(uint64_t v)
{
    return v <= g_r ? g_id1[v] : g_id2[g_N / v];
}

/* type: 0 ramified (2,5), 1 inert, 2 principal split, 3 nonprincipal split */
static int gpe(size_t i, int e, int which)
{
    int t = g_type[i];
    if (t == 1) return (e % 2 == 0) ? 1 : 0;
    if (which == 0 || t == 2) return 1;
    return (e & 1) ? -1 : 1;
}

static int64_t rec(uint64_t v, size_t j, int which)
{
    int64_t res = g_G[which][idx(v)] - g_Gp[which][j];
    size_t i;
    for (i = j; i < g_np && g_pr[i] * g_pr[i] <= v; i++) {
        uint64_t p = g_pr[i], pe = p;
        int e = 1;
        while (pe <= v / p) {
            int c = gpe(i, e, which);
            if (c) res += (int64_t)c * rec(v / pe, i + 1, which);
            res += gpe(i, e + 1, which);
            pe *= p;
            e++;
        }
    }
    return res;
}

static uint64_t count_upto(uint64_t N)
{
    uint64_t r = isq(N);
    size_t m = 0, k, i;
    g_N = N; g_r = r;
    g_w = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(2 * r + 3));
    g_id1 = (size_t *)malloc(sizeof(size_t) * (size_t)(r + 2));
    g_id2 = (size_t *)malloc(sizeof(size_t) * (size_t)(r + 2));
    {
        uint64_t q = 1;
        while (q <= N) {
            uint64_t v = N / q;
            g_w[m] = v;
            if (v <= r) g_id1[v] = m; else g_id2[N / v] = m;
            m++;
            q = N / v + 1;
        }
    }
    int64_t *S0 = (int64_t *)malloc(sizeof(int64_t) * m);
    int64_t *S1 = (int64_t *)malloc(sizeof(int64_t) * m);
    int64_t *S2 = (int64_t *)malloc(sizeof(int64_t) * m);
    int64_t *S3 = (int64_t *)malloc(sizeof(int64_t) * m);
    for (k = 0; k < m; k++) {
        uint64_t v = g_w[k];
        S0[k] = (int64_t)v - 1;
        S1[k] = prefc(v, T40, 40) - 1;
        S2[k] = prefc(v, T5, 5) - 1;
        S3[k] = prefc(v, T8, 8) - 1;
    }
    /* primes up to r */
    unsigned char *comp = (unsigned char *)calloc((size_t)(r + 2), 1);
    g_pr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(r + 2));
    g_np = 0;
    {
        uint64_t p, q;
        for (p = 2; p <= r; p++) {
            if (comp[p]) continue;
            g_pr[g_np++] = p;
            for (q = p * p; q <= r; q += p) comp[q] = 1;
        }
    }
    free(comp);
    for (i = 0; i < g_np; i++) {
        uint64_t p = g_pr[i], p2 = p * p;
        size_t ip = idx(p - 1);
        int c1 = T40[p % 40], c2 = T5[p % 5], c3 = T8[p % 8];
        int64_t b0 = S0[ip], b1 = S1[ip], b2 = S2[ip], b3 = S3[ip];
        for (k = 0; k < m && g_w[k] >= p2; k++) {
            size_t t = idx(g_w[k] / p);
            S0[k] -= S0[t] - b0;
            if (c1) S1[k] -= (int64_t)c1 * (S1[t] - b1);
            if (c2) S2[k] -= (int64_t)c2 * (S2[t] - b2);
            if (c3) S3[k] -= (int64_t)c3 * (S3[t] - b3);
        }
    }
    g_G[0] = (int64_t *)malloc(sizeof(int64_t) * m);
    g_G[1] = (int64_t *)malloc(sizeof(int64_t) * m);
    for (k = 0; k < m; k++) {
        uint64_t v = g_w[k];
        int64_t r2 = v >= 2 ? 1 : 0, r5 = v >= 5 ? 1 : 0;
        int64_t pi = S0[k] - r2 - r5;
        int64_t PpQ = (pi + S1[k]) / 2;
        int64_t PmQ = ((S2[k] + r2) + (S3[k] + r5)) / 2;
        g_G[0][k] = PpQ + r2 + r5;
        g_G[1][k] = PmQ - r2 - r5;
    }
    free(S0); free(S1); free(S2); free(S3);
    g_type = (int *)malloc(sizeof(int) * (g_np + 1));
    g_Gp[0] = (int64_t *)malloc(sizeof(int64_t) * (g_np + 1));
    g_Gp[1] = (int64_t *)malloc(sizeof(int64_t) * (g_np + 1));
    g_Gp[0][0] = 0; g_Gp[1][0] = 0;
    for (i = 0; i < g_np; i++) {
        uint64_t p = g_pr[i];
        int t;
        if (p == 2 || p == 5) t = 0;
        else if (T40[p % 40] == -1) t = 1;
        else if (T5[p % 5] == 1) t = 2;
        else t = 3;
        g_type[i] = t;
        g_Gp[0][i + 1] = g_Gp[0][i] + gpe(i, 1, 0);
        g_Gp[1][i + 1] = g_Gp[1][i] + gpe(i, 1, 1);
    }
    int64_t t0 = 1 + rec(N, 0, 0);
    int64_t t1 = 1 + rec(N, 0, 1);
    free(g_G[0]); free(g_G[1]); free(g_Gp[0]); free(g_Gp[1]);
    free(g_type); free(g_pr); free(g_w); free(g_id1); free(g_id2);
    return (uint64_t)((t0 + t1) / 2);
}

seq_t a(idx_t n)
{
    static uint64_t memo[64];
    static int have[64];
    if (n >= 63) return (seq_t)0;
    if (!have[n]) {
        memo[n] = count_upto((uint64_t)1 << n);
        have[n] = 1;
    }
    return (seq_t)memo[n];
}
