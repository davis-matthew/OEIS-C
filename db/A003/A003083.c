/*
 * A003083  Sum a(n) x^n / n = log (1 + Sum g(n) x^n ), where g(n) is # graphs
 *          on n nodes (A000088).
 * https://oeis.org/A003083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*g(n) - Sum_{k=1..n-1} a(k)*g(n-k), where g = A000088
 *            (g(0)=1) is computed exactly by Polya counting: g(n) = (1/n!) *
 *            Sum_{partitions p of n} (n!/z(p)) * 2^(Sum_{i<j} gcd(p_i,p_j) +
 *            Sum_i floor(p_i/2))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   A_grow, g3083_divs, g3083_gcd, g3083_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/A_grow.h"
#include "../utils/g3083_divs.h"
#include "../utils/g3083_gcd.h"
#include "../utils/g3083_trim.h"

static int g3083_n;
static uint32_t *g3083_fact, *g3083_tmp, *g3083_acc;
static size_t g3083_L;
static int g3083_part[1024], g3083_mult[1024];

static void g3083_leaf(int nd, uint64_t e) {
    size_t len = g3083_L, i, ls, tot = g3083_L + 4;
    uint64_t ch = 1, carry = 0;
    unsigned bs;
    int j, t;
    memcpy(g3083_tmp, g3083_fact, g3083_L * sizeof(uint32_t));
    len = g3083_trim(g3083_tmp, len);
    for (j = 0; j < nd; j++) {
        for (t = 1; t <= 2 * g3083_mult[j]; t++) {
            uint64_t f = (t <= g3083_mult[j]) ? (uint64_t)g3083_part[j] : (uint64_t)(t - g3083_mult[j]);
            if (f <= 1) continue;
            if (ch * f > 0xFFFFFFFFull) {
                g3083_divs(g3083_tmp, len, (uint32_t)ch);
                len = g3083_trim(g3083_tmp, len);
                ch = f;
            } else ch *= f;
        }
    }
    if (ch > 1) { g3083_divs(g3083_tmp, len, (uint32_t)ch); len = g3083_trim(g3083_tmp, len); }
    ls = (size_t)(e / 32);
    bs = (unsigned)(e % 32);
    for (i = 0; i <= len && ls + i < tot; i++) {
        uint64_t lo = (i < len) ? ((uint64_t)g3083_tmp[i] << bs) & 0xFFFFFFFFull : 0;
        uint64_t hi = (bs && i > 0) ? ((uint64_t)g3083_tmp[i - 1] >> (32 - bs)) : 0;
        uint64_t s = (uint64_t)g3083_acc[ls + i] + (lo | hi) + carry;
        g3083_acc[ls + i] = (uint32_t)s;
        carry = s >> 32;
    }
    for (i = ls + len + 1; carry && i < tot; i++) {
        uint64_t s = (uint64_t)g3083_acc[i] + carry;
        g3083_acc[i] = (uint32_t)s;
        carry = s >> 32;
    }
}

static void g3083_rec(int rem, int maxk, int nd, uint64_t e) {
    int k, j, m;
    if (rem == 0) { g3083_leaf(nd, e); return; }
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        uint64_t cross = 0;
        for (j = 0; j < nd; j++) cross += (uint64_t)g3083_mult[j] * (uint64_t)g3083_gcd(k, g3083_part[j]);
        for (m = 1; m * k <= rem; m++) {
            uint64_t e2 = e + (uint64_t)m * (uint64_t)(k / 2)
                        + (uint64_t)k * (uint64_t)m * (uint64_t)(m - 1) / 2
                        + (uint64_t)m * cross;
            g3083_part[nd] = k;
            g3083_mult[nd] = m;
            g3083_rec(rem - m * k, k - 1, nd + 1, e2);
        }
    }
}

static seq_t g3083_graphs(int n) {
    uint64_t bits = 64;
    int k, b;
    size_t len, i;
    seq_t v = 0;
    uint64_t ch = 1;
    if (n <= 1) return 1;
    g3083_n = n;
    bits += (uint64_t)n * (uint64_t)(n - 1) / 2;
    for (k = 2; k <= n; k++) { int t = k; while (t) { bits++; t >>= 1; } }
    g3083_L = (size_t)(bits / 32) + 2;
    g3083_fact = (uint32_t *)calloc(g3083_L, sizeof(uint32_t));
    g3083_tmp = (uint32_t *)calloc(g3083_L, sizeof(uint32_t));
    g3083_acc = (uint32_t *)calloc(g3083_L + 4, sizeof(uint32_t));
    g3083_fact[0] = 1;
    for (k = 2; k <= n; k++) {
        uint64_t c = 0;
        for (i = 0; i < g3083_L; i++) {
            uint64_t p = (uint64_t)g3083_fact[i] * (uint64_t)k + c;
            g3083_fact[i] = (uint32_t)p;
            c = p >> 32;
        }
    }
    g3083_rec(n, n, 0, 0);
    len = g3083_trim(g3083_acc, g3083_L + 4);
    for (k = 2; k <= n; k++) {
        if (ch * (uint64_t)k > 0xFFFFFFFFull) {
            g3083_divs(g3083_acc, len, (uint32_t)ch);
            len = g3083_trim(g3083_acc, len);
            ch = (uint64_t)k;
        } else ch *= (uint64_t)k;
    }
    if (ch > 1) { g3083_divs(g3083_acc, len, (uint32_t)ch); len = g3083_trim(g3083_acc, len); }
    for (i = len; i-- > 0;) {
        for (b = 3; b >= 0; b--) {
            v *= 256;
            v += (unsigned)((g3083_acc[i] >> (8 * b)) & 0xFFu);
        }
    }
    free(g3083_fact);
    free(g3083_tmp);
    free(g3083_acc);
    return v;
}

seq_t a(idx_t n) {
    static seq_t *G = NULL, *A = NULL;
    static idx_t have = 0;
    if (n >= have) {
        idx_t i, k;
        G = A_grow(G, have, n + 1);
        A = A_grow(A, have, n + 1);
        for (i = have; i <= n; i++) {
            G[i] = g3083_graphs((int)i);
            if (i >= 1) {
                seq_t s = 0;
                for (k = 1; k < i; k++) s += A[k] * G[i - k];
                A[i] = (seq_t)i * G[i] - s;
            } else {
                A[i] = 0;
            }
        }
        have = n + 1;
    }
    return A[n];
}
