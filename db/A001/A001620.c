/*
 * A001620  Decimal expansion of Euler's constant (or the Euler-Mascheroni
 *          constant), gamma.
 * https://oeis.org/A001620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, nice
 * Formula:   a(n) = n-th decimal digit of gamma = lim_{k->oo} (H_k - log(k)),
 *            computed with the Brent-McMillan algorithm: gamma = U/V - log(N)
 *            with U = Sum_{k>=1} (N^k/k!)^2 * H_k, V = Sum_{k>=0} (N^k/k!)^2,
 *            error O(exp(-4N)), in fixed-point arithmetic; log(N) for
 *            5-smooth N from log(2) = 2*atanh(1/3), log(3/...
 * Range:     u8 n <= 8637, u16 n <= 8637, u32 n <= 8637, u64 n <= 8637,
 *            u128 n <= 8637, big any n
 * Helpers:   bm_add, bm_atanh, bm_div, bm_divmod, bm_divto, bm_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bm_add.h"
#include "../utils/bm_atanh.h"
#include "../utils/bm_div.h"
#include "../utils/bm_divmod.h"
#include "../utils/bm_divto.h"
#include "../utils/bm_mul.h"

static char *g_gdig = NULL;
static idx_t g_gndig = 0;

static void bm_gamma(size_t D) {
    size_t Dw = D + 30;
    size_t FL = (Dw * 3322 / 1000 + 64) / 32 + 2;
    uint64_t Nt = (uint64_t)FL * 5546 / 1000 + 2;
    uint64_t N = 0;
    unsigned ea = 0, eb = 0, ec = 0;
    for (uint64_t p3 = 1, b = 0; p3 <= 2 * Nt; p3 *= 3, b++)
        for (uint64_t p5 = p3, c = 0; p5 <= 2 * Nt; p5 *= 5, c++) {
            uint64_t v = p5;
            unsigned e = 0;
            while (v < Nt) { v *= 2; e++; }
            if (N == 0 || v < N) { N = v; ea = e; eb = (unsigned)b; ec = (unsigned)c; }
        }
    uint64_t N2 = N * N;
    size_t IL = (size_t)((N * 2886 / 1000 + 64) / 32) + 6;
    size_t L = FL + IL;
    uint32_t *A = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *V = (uint32_t *)calloc(L, sizeof(uint32_t));
    B[FL] = 1;
    size_t len = FL + 1;
    size_t stopLen = (N * 2885 / 1000 > 96) ? (size_t)((N * 2885 / 1000 - 96) / 32) : 0;
    const uint64_t LIM = (uint64_t)1 << 32;
    for (uint64_t k = 1;; k++) {
        uint64_t k2 = k * k;
        size_t m = len + 3;
        if (N2 + k < LIM && k2 < LIM) {
            uint64_t cv = 0, cb = 0;
            for (size_t i = 0; i < m; i++) {
                uint64_t b = B[i];
                uint64_t t = (uint64_t)V[i] + b + cv;
                V[i] = (uint32_t)t;
                cv = t >> 32;
                t = b * N2 + cb;
                B[i] = (uint32_t)t;
                cb = t >> 32;
            }
            for (size_t i = m; cv && i < L; i++) {
                uint64_t t = (uint64_t)V[i] + cv;
                V[i] = (uint32_t)t;
                cv = t >> 32;
            }
            bm_div(B, m, (uint32_t)k2);
            uint64_t cs = 0, ca = 0;
            for (size_t i = 0; i < m; i++) {
                uint64_t a0 = A[i];
                uint64_t t = (uint64_t)S[i] + a0 + cs;
                S[i] = (uint32_t)t;
                cs = t >> 32;
                t = a0 * N2 + (uint64_t)B[i] * k + ca;
                A[i] = (uint32_t)t;
                ca = t >> 32;
            }
            for (size_t i = m; cs && i < L; i++) {
                uint64_t t = (uint64_t)S[i] + cs;
                S[i] = (uint32_t)t;
                cs = t >> 32;
            }
            bm_div(A, m, (uint32_t)k2);
        } else {
            bm_add(V, L, B, m);
            bm_mul(B, m, (uint32_t)N);
            bm_mul(B, m, (uint32_t)N);
            bm_div(B, m, (uint32_t)k);
            bm_div(B, m, (uint32_t)k);
            bm_add(S, L, A, m);
            bm_mul(A, m, (uint32_t)N);
            bm_mul(A, m, (uint32_t)N);
            bm_div(A, m, (uint32_t)k);
            bm_add(A, m, B, m);
            bm_div(A, m, (uint32_t)k);
        }
        len = m;
        while (len && A[len - 1] == 0 && B[len - 1] == 0) len--;
        if (k > N && len <= stopLen) break;
    }
    bm_add(S, L, A, L);
    bm_add(V, L, B, L);

    /* Q = S/V scaled by 2^(32*FL) */
    size_t vlen = L, slen = L;
    while (vlen && V[vlen - 1] == 0) vlen--;
    while (slen && S[slen - 1] == 0) slen--;
    size_t drop = vlen > FL + 4 ? vlen - (FL + 4) : 0;
    size_t mu = FL + slen - drop, nv = vlen - drop;
    uint32_t *un = (uint32_t *)calloc(mu, sizeof(uint32_t));
    for (size_t i = 0; i < slen - drop; i++) un[FL + i] = S[drop + i];
    size_t qn = mu - nv + 1;
    uint32_t *q = (uint32_t *)calloc(qn, sizeof(uint32_t));
    bm_divmod(q, un, mu, V + drop, nv);

    /* log(N) = ea*log2 + eb*log3 + ec*log5 */
    size_t M = FL + 2;
    uint32_t *L2 = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *L3 = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *L5 = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(M, sizeof(uint32_t));
    uint32_t *lnN = (uint32_t *)calloc(M, sizeof(uint32_t));
    bm_atanh(L2, M, FL, 3);
    bm_mul(L2, M, 2);
    bm_atanh(L3, M, FL, 5);
    bm_mul(L3, M, 2);
    bm_add(L3, M, L2, M);
    bm_atanh(L5, M, FL, 9);
    bm_mul(L5, M, 2);
    bm_add(L5, M, L2, M);
    bm_add(L5, M, L2, M);
    memcpy(T, L2, M * sizeof(uint32_t)); bm_mul(T, M, ea); bm_add(lnN, M, T, M);
    memcpy(T, L3, M * sizeof(uint32_t)); bm_mul(T, M, eb); bm_add(lnN, M, T, M);
    memcpy(T, L5, M * sizeof(uint32_t)); bm_mul(T, M, ec); bm_add(lnN, M, T, M);

    /* gamma = Q - log(N) */
    uint64_t br = 0;
    for (size_t i = 0; i < qn; i++) {
        uint64_t sub = (i < M ? (uint64_t)lnN[i] : 0) + br;
        uint64_t qi = q[i];
        q[i] = (uint32_t)(qi - sub);
        br = qi < sub;
    }

    char *dig = (char *)malloc(Dw + 9);
    for (size_t out = 0; out < Dw; out += 9) {
        uint32_t c = bm_mul(q, FL, 1000000000u);
        for (int d = 8; d >= 0; d--) {
            dig[out + (size_t)d] = (char)(c % 10);
            c /= 10;
        }
    }

    free(A); free(B); free(S); free(V);
    free(un); free(q);
    free(L2); free(L3); free(L5); free(T); free(lnN);
    free(g_gdig);
    g_gdig = dig;
    g_gndig = (idx_t)D;
}

seq_t a(idx_t n) {
    if (n >= g_gndig) {
        size_t want = (size_t)n + 1;
        size_t grow = (size_t)g_gndig + (size_t)g_gndig / 4;
        if (want < grow) want = grow;
        if (want < 256) want = 256;
        bm_gamma(want);
    }
    return (seq_t)g_gdig[n];
}
