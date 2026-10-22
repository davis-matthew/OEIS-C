/*
 * A005639  Number of self-converse oriented graphs with n nodes.
 * https://oeis.org/A005639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n!) * Sum_{partitions p of n} (n!/z(p)) * 3^e(p) (de
 *            Bruijn counting of graphs isomorphic to their converse), where p
 *            has m_k cycles of length k, z(p) = Prod k^m_k * m_k!, e(p) =
 *            Sum_k m_k*w(k) + Sum_k C(m_k,2)*s(k) + Sum_{k<l} m_k*m_l*t(k,l),
 *            w(k) = 0 (k odd), k/2 (k == 2 mod 4), (k-2)...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   bn_divsmall, w_in
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/bn_divsmall.h"
#include "../utils/w_in.h"

static uint32_t *g_buf;
static uint32_t *g_cE;
static size_t g_L;
static idx_t g_pk[4096];
static idx_t g_pm[4096];
static size_t g_np;

static uint64_t g_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t t_pair(uint64_t k, uint64_t l) {
    if ((k & 1) && (l & 1)) return 0;
    return g_gcd(k, l);
}

static void bn_add(uint32_t *x, const uint32_t *y, size_t len) {
    uint64_t c = 0;
    for (size_t i = 0; i < len; i++) {
        c += (uint64_t)x[i] + y[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void rec(idx_t rem, idx_t k, size_t depth, uint64_t e) {
    const uint32_t *coef = g_buf + depth * g_L;
    if (rem == 0) {
        bn_add(g_cE + e * g_L, coef, g_L);
        return;
    }
    if (k == 0) return;
    uint32_t *nc = g_buf + (depth + 1) * g_L;
    memcpy(nc, coef, g_L * sizeof(uint32_t));
    if (k > 1) rec(rem, k - 1, depth + 1, e);
    uint64_t cross = 0;
    for (size_t p = 0; p < g_np; p++) cross += (uint64_t)g_pm[p] * t_pair(k, g_pk[p]);
    uint64_t sk = (k & 1) ? 0 : k;
    uint64_t wk = w_in(k);
    for (idx_t m = 1; m * k <= rem; m++) {
        bn_divsmall(nc, g_L, (uint32_t)(k * m));
        if (k == 1 && m * k != rem) continue;
        uint64_t e2 = e + m * wk + (m * (m - 1) / 2) * sk + m * cross;
        g_pk[g_np] = k;
        g_pm[g_np] = m;
        g_np++;
        rec(rem - m * k, k - 1, depth + 1, e2);
        g_np--;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t bits = 1;
    for (idx_t j = 2; j <= n; j++) {
        uint64_t b = 0, v = j;
        while (v) { b++; v >>= 1; }
        bits += b;
    }
    g_L = (size_t)(bits / 32 + 2);
    uint64_t emax = (uint64_t)n * n / 2 + 1;

    g_buf = (uint32_t *)calloc((size_t)(n + 3) * g_L, sizeof(uint32_t));
    g_cE = (uint32_t *)calloc((size_t)(emax + 1) * g_L, sizeof(uint32_t));

    /* coefficient at depth 0: n! */
    g_buf[0] = 1;
    for (idx_t j = 2; j <= n; j++) {
        uint64_t c = 0;
        for (size_t i = 0; i < g_L; i++) {
            c += (uint64_t)g_buf[i] * j;
            g_buf[i] = (uint32_t)c;
            c >>= 32;
        }
    }
    g_np = 0;
    rec(n, n, 0, 0);

    size_t LT = g_L + (size_t)(emax * 2 / 32) + 4;
    uint32_t *T = (uint32_t *)calloc(LT, sizeof(uint32_t));
    for (uint64_t e = emax + 1; e-- > 0;) {
        uint64_t c = 0;
        const uint32_t *ce = g_cE + e * g_L;
        for (size_t i = 0; i < LT; i++) {
            c += (uint64_t)T[i] * 3 + (i < g_L ? ce[i] : 0);
            T[i] = (uint32_t)c;
            c >>= 32;
        }
    }
    for (idx_t j = 2; j <= n; j++) bn_divsmall(T, LT, (uint32_t)j);

    seq_t r = 0;
    for (size_t i = LT; i-- > 0;) {
        uint32_t limb = T[i];
        for (int s = 24; s >= 0; s -= 8) {
            r = (seq_t)(r * 256);
            r += (seq_t)((limb >> s) & 255u);
        }
    }
    free(T);
    free(g_cE);
    free(g_buf);
    g_cE = NULL;
    g_buf = NULL;
    return r;
}
