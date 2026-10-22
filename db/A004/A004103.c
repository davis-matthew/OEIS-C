/*
 * A004103  Number of nets on n unlabeled nodes.
 * https://oeis.org/A004103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n!) * Sum_{partitions lambda of n} (n!/z(lambda)) *
 *            2^c(lambda) * 3^e(lambda), where z(lambda) = Prod_k k^{m_k}
 *            m_k!, c(lambda) = Sum_k m_k (vertex cycles), and e(lambda) =
 *            Sum_k [ m_k*floor(k/2) + k*m_k*(m_k-1)/2 ] + Sum_{i<j}
 *            m_i*m_j*gcd(i,j) (edge cycles). Burnside over S_n: each ver...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   sg_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sg_gcd.h"

static int g_Lc;
static uint32_t *g_buf;
static uint32_t *g_C;
static int *g_pk, *g_pm;

static void bn_mul_small(uint32_t *a, int L, uint32_t m) {
    uint64_t carry = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void bn_div_small(uint32_t *a, int L, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void bn_add(uint32_t *dst, int Ld, const uint32_t *src, int Ls) {
    uint64_t carry = 0;
    int i;
    for (i = 0; i < Ld; i++) {
        uint64_t t = (uint64_t)dst[i] + carry + (i < Ls ? src[i] : 0);
        dst[i] = (uint32_t)t;
        carry = t >> 32;
        if (i >= Ls && carry == 0) break;
    }
}

static void rec(int depth, int rem, int maxk, long long e) {
    int Lc = g_Lc;
    uint32_t *cur = g_buf + (size_t)depth * Lc;
    uint32_t *w = g_buf + (size_t)(depth + 1) * Lc;
    int k, j, m;
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        long long cross = 0;
        for (j = 0; j < depth; j++) cross += (long long)g_pm[j] * sg_gcd(k, g_pk[j]);
        memcpy(w, cur, sizeof(uint32_t) * (size_t)Lc);
        for (m = 1; m * k <= rem; m++) {
            long long ne;
            bn_mul_small(w, Lc, 2);
            bn_div_small(w, Lc, (uint32_t)k);
            bn_div_small(w, Lc, (uint32_t)m);
            ne = e + (long long)m * (k / 2) + (long long)k * m * (m - 1) / 2 + (long long)m * cross;
            if (rem == m * k) {
                bn_add(g_C + (size_t)ne * Lc, Lc, w, Lc);
            } else {
                g_pk[depth] = k;
                g_pm[depth] = m;
                rec(depth + 1, rem - m * k, k - 1, ne);
            }
        }
    }
}

seq_t a(idx_t n) {
    int N = (int)n, i, bits, Lc, L;
    long long E, ei;
    uint32_t *S;
    seq_t r = 0;
    if (N == 0) return (seq_t)1;

    bits = 0;
    for (i = 2; i <= N; i++) { int b = 0, t = i; while (t) { b++; t >>= 1; } bits += b; }
    bits += 2 * N + 64;
    Lc = bits / 32 + 2;
    g_Lc = Lc;
    E = (long long)N * (N - 1) / 2;
    L = Lc + (int)(E / 16) + 2;

    g_buf = (uint32_t *)calloc((size_t)(N + 3) * Lc, sizeof(uint32_t));
    g_C = (uint32_t *)calloc((size_t)(E + 1) * Lc, sizeof(uint32_t));
    g_pk = (int *)calloc((size_t)N + 2, sizeof(int));
    g_pm = (int *)calloc((size_t)N + 2, sizeof(int));
    S = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));

    g_buf[0] = 1;
    for (i = 2; i <= N; i++) bn_mul_small(g_buf, Lc, (uint32_t)i);

    rec(0, N, N, 0);

    for (ei = E; ei >= 0; ei--) {
        bn_mul_small(S, L, 3);
        bn_add(S, L, g_C + (size_t)ei * Lc, Lc);
    }
    for (i = 2; i <= N; i++) bn_div_small(S, L, (uint32_t)i);

    for (i = L - 1; i >= 0; i--) {
        r = r * 65536;
        r = r + (int)(S[i] >> 16);
        r = r * 65536;
        r = r + (int)(S[i] & 0xFFFFu);
    }

    free(g_buf); free(g_C); free(g_pk); free(g_pm); free(S);
    return r;
}
