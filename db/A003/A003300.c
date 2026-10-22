/*
 * A003300  Denominators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   c(n) = A003299(n)/a(n) with c(0) = 0, c(1) = 1 and
 *            36*n*(n+1)*(2n+1)*c(n+1) = 4*n*(20*n^2+1)*c(n) -
 *            (2*n-1)^3*c(n-1); a(n) = denominator of c(n). The fraction is
 *            kept reduced, with the denominator stored as prime exponents and
 *            the numerator as a bignum
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   g3300_inv, g3300_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/g3300_inv.h"
#include "../utils/g3300_pw.h"

typedef struct { uint32_t *d; size_t len, cap; } g3300_big;

static void g3300_reserve(g3300_big *x, size_t need) {
    if (need > x->cap) {
        size_t c = x->cap ? x->cap : 16;
        while (c < need) c *= 2;
        x->d = (uint32_t *)realloc(x->d, c * sizeof(uint32_t));
        x->cap = c;
    }
}
static void g3300_set(g3300_big *x, uint32_t v) {
    g3300_reserve(x, 1);
    x->len = 0;
    if (v) { x->d[0] = v; x->len = 1; }
}
static void g3300_copy(g3300_big *dst, const g3300_big *src) {
    g3300_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}
static void g3300_mul(g3300_big *x, uint64_t m) {
    unsigned __int128 carry = 0;
    size_t i;
    if (m == 0 || x->len == 0) { x->len = 0; return; }
    for (i = 0; i < x->len; i++) {
        carry += (unsigned __int128)x->d[i] * m;
        x->d[i] = (uint32_t)carry;
        carry >>= 32;
    }
    while (carry) {
        g3300_reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)carry;
        carry >>= 32;
    }
}
static void g3300_mulpow(g3300_big *x, uint64_t p, int d) {
    uint64_t m = 1;
    while (d-- > 0) {
        if (m > UINT64_MAX / p) { g3300_mul(x, m); m = 1; }
        m *= p;
    }
    if (m > 1) g3300_mul(x, m);
}
/* a -= b, requires a >= b */
static void g3300_sub(g3300_big *a, const g3300_big *b) {
    int64_t br = 0;
    size_t i;
    for (i = 0; i < a->len; i++) {
        int64_t t = (int64_t)a->d[i] - br - (i < b->len ? (int64_t)b->d[i] : 0);
        if (t < 0) { t += (int64_t)1 << 32; br = 1; } else br = 0;
        a->d[i] = (uint32_t)t;
    }
    while (a->len && a->d[a->len - 1] == 0) a->len--;
}
static uint32_t g3300_mod(const g3300_big *x, uint32_t p) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i--) r = ((r << 32) | x->d[i]) % p;
    return (uint32_t)r;
}
static void g3300_div(g3300_big *x, uint32_t p) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i--) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / p);
        r = cur % p;
    }
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

static idx_t g_cur = 0;
static g3300_big g_P0, g_P1, g_X, g_Y;
static uint32_t *g_pr, *g_r0, *g_r1, *g_xr;
static int *g_e0, *g_e1, *g_en, *g_l1i, *g_l1d, *g_l0i, *g_l0d, *g_ci, *g_cc;
static size_t g_np = 0, g_pcap = 0;
static uint64_t g_lim = 1;

static void g3300_addprime(uint32_t p) {
    if (g_np == g_pcap) {
        size_t c = g_pcap ? 2 * g_pcap : 64;
        g_pr = (uint32_t *)realloc(g_pr, c * sizeof(uint32_t));
        g_r0 = (uint32_t *)realloc(g_r0, c * sizeof(uint32_t));
        g_r1 = (uint32_t *)realloc(g_r1, c * sizeof(uint32_t));
        g_xr = (uint32_t *)realloc(g_xr, c * sizeof(uint32_t));
        g_e0 = (int *)realloc(g_e0, c * sizeof(int));
        g_e1 = (int *)realloc(g_e1, c * sizeof(int));
        g_en = (int *)realloc(g_en, c * sizeof(int));
        g_l1i = (int *)realloc(g_l1i, c * sizeof(int));
        g_l1d = (int *)realloc(g_l1d, c * sizeof(int));
        g_l0i = (int *)realloc(g_l0i, c * sizeof(int));
        g_l0d = (int *)realloc(g_l0d, c * sizeof(int));
        g_ci = (int *)realloc(g_ci, c * sizeof(int));
        g_cc = (int *)realloc(g_cc, c * sizeof(int));
        g_pcap = c;
    }
    g_pr[g_np] = p;
    g_e0[g_np] = 0;
    g_e1[g_np] = 0;
    g_r0[g_np] = g3300_mod(&g_P0, p);
    g_r1[g_np] = g3300_mod(&g_P1, p);
    g_np++;
}

static void g3300_ensure(uint64_t upto) {
    while (g_lim < upto) {
        uint64_t v = ++g_lim;
        size_t i;
        int isp = 1;
        for (i = 0; i < g_np && (uint64_t)g_pr[i] * g_pr[i] <= v; i++)
            if (v % g_pr[i] == 0) { isp = 0; break; }
        if (isp) g3300_addprime((uint32_t)v);
    }
}

static void g3300_addfac(uint64_t x, int k) {
    size_t i;
    for (i = 0; i < g_np && (uint64_t)g_pr[i] * g_pr[i] <= x; i++)
        while (x % g_pr[i] == 0) { x /= g_pr[i]; g_en[i] += k; }
    if (x > 1) {
        size_t lo = 0, hi = g_np - 1;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (g_pr[mid] < x) lo = mid + 1; else hi = mid;
        }
        g_en[lo] += k;
    }
}

static void g3300_reset(void) {
    size_t i;
    g3300_set(&g_P0, 0);
    g3300_set(&g_P1, 1);
    for (i = 0; i < g_np; i++) {
        g_e0[i] = 0; g_e1[i] = 0;
        g_r0[i] = 0; g_r1[i] = 1 % g_pr[i];
    }
    g_cur = 1;
}

/* from c(n-1), c(n) compute c(n+1) */
static void g3300_step(idx_t n) {
    size_t i, np;
    int n1 = 0, n0 = 0, nc = 0, j;
    g3300_ensure(2 * (uint64_t)n + 1);
    np = g_np;
    for (i = 0; i < np; i++) {
        int m = g_e0[i] > g_e1[i] ? g_e0[i] : g_e1[i];
        g_en[i] = m;
        if (m > g_e1[i]) { g_l1i[n1] = (int)i; g_l1d[n1] = m - g_e1[i]; n1++; }
        if (m > g_e0[i]) { g_l0i[n0] = (int)i; g_l0d[n0] = m - g_e0[i]; n0++; }
    }
    g3300_addfac(2, 2);
    g3300_addfac(3, 2);
    g3300_addfac(n, 1);
    g3300_addfac(n + 1, 1);
    g3300_addfac(2 * (uint64_t)n + 1, 1);

    g3300_copy(&g_X, &g_P1);
    g3300_mul(&g_X, 4 * (uint64_t)n);
    g3300_mul(&g_X, 20 * (uint64_t)n * n + 1);
    for (j = 0; j < n1; j++) g3300_mulpow(&g_X, g_pr[g_l1i[j]], g_l1d[j]);
    g3300_copy(&g_Y, &g_P0);
    g3300_mul(&g_Y, 2 * (uint64_t)n - 1);
    g3300_mul(&g_Y, 2 * (uint64_t)n - 1);
    g3300_mul(&g_Y, 2 * (uint64_t)n - 1);
    for (j = 0; j < n0; j++) g3300_mulpow(&g_Y, g_pr[g_l0i[j]], g_l0d[j]);
    g3300_sub(&g_X, &g_Y);

    for (i = 0; i < np; i++) {
        uint64_t p = g_pr[i], nn = n % p, A, B, t, R1 = 1 % p, R0 = 1 % p;
        A = (4 * nn % p) * ((20 * nn % p * nn + 1) % p) % p;
        t = (2 * nn + p - 1) % p;
        B = t * t % p * t % p;
        for (j = 0; j < n1; j++) R1 = R1 * g3300_pw(g_pr[g_l1i[j]], g_l1d[j], p) % p;
        for (j = 0; j < n0; j++) R0 = R0 * g3300_pw(g_pr[g_l0i[j]], g_l0d[j], p) % p;
        g_xr[i] = (uint32_t)((A * g_r1[i] % p * R1 % p + p - B * g_r0[i] % p * R0 % p) % p);
    }

    for (i = 0; i < np; i++) {
        if (g_en[i] > 0 && g_xr[i] == 0) {
            int cnt = 0;
            while (g_en[i] > 0 && g3300_mod(&g_X, g_pr[i]) == 0) {
                g3300_div(&g_X, g_pr[i]);
                g_en[i]--;
                cnt++;
            }
            if (cnt) { g_ci[nc] = (int)i; g_cc[nc] = cnt; nc++; }
        }
    }
    if (nc) {
        for (i = 0; i < np; i++) {
            uint64_t p = g_pr[i], C = 1 % p;
            for (j = 0; j < nc; j++) C = C * g3300_pw(g_pr[g_ci[j]], g_cc[j], p) % p;
            if (C) g_xr[i] = (uint32_t)((uint64_t)g_xr[i] * g3300_inv(C, p) % p);
        }
        for (j = 0; j < nc; j++) g_xr[g_ci[j]] = g3300_mod(&g_X, g_pr[g_ci[j]]);
    }

    {
        uint32_t *tr = g_r0; int *te = g_e0; g3300_big tb = g_P0;
        g_r0 = g_r1; g_r1 = g_xr; g_xr = tr;
        g_e0 = g_e1; g_e1 = g_en; g_en = te;
        g_P0 = g_P1; g_P1 = g_X; g_X = tb;
    }
}

seq_t a(idx_t n) {
    seq_t r = 1;
    size_t i;
    int k;
    if (n < 2) return r;
    if (g_cur == 0 || n < g_cur) g3300_reset();
    while (g_cur < n) { g3300_step(g_cur); g_cur++; }
    for (i = 0; i < g_np; i++)
        for (k = 0; k < g_e1[i]; k++) r = r * (idx_t)g_pr[i];
    return r;
}
