/*
 * A001435  Number of connected graphs with n nodes and n+1 edges.
 * https://oeis.org/A001435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count unlabeled connected graphs of complexity 2 by their
 *            2-core. With rooted trees T(x) (A000081) hung on every core
 *            vertex: P = 1/(1-T) (paths), Ps = 1 + T + T(x^2)*Ps (palindromic
 *            paths), U = 1 + T + T(x^2)*U + D2(T)*P (paths up to reversal), L
 *            = U - 1 - T (loops), E = T*L, Np = U - Ps, D2/D3...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t bin2(seq_t x) {
    if (x % 2 == 0) return (x / 2) * (x - 1);
    return x * ((x - 1) / 2);
}

static seq_t bin3(seq_t x) {
    seq_t p, q, r;
    if (x < 3) return 0;
    p = x; q = x - 1; r = x - 2;
    if (p % 3 == 0) p /= 3; else if (q % 3 == 0) q /= 3; else r /= 3;
    if (p % 2 == 0) p /= 2; else q /= 2;
    return p * q * r;
}

/* H1,H2,H3: (multi)sets of 1,2,3 objects drawn from class A */
static void choose3(const seq_t *A, long long N, int multi, int need3,
                    seq_t *H1, seq_t *H2, seq_t *H3) {
    long long k, w;
    for (w = 0; w <= N; w++) { H1[w] = 0; H2[w] = 0; H3[w] = 0; }
    for (k = 0; k <= N; k++) {
        seq_t c1, c2, c3;
        if (A[k] == 0) continue;
        c1 = A[k];
        c2 = multi ? bin2(A[k] + 1) : bin2(A[k]);
        c3 = 0;
        if (need3) c3 = multi ? bin3(A[k] + 2) : bin3(A[k]);
        for (w = N; w >= 0; w--) {
            if (need3) {
                if (w >= k) H3[w] += c1 * H2[w - k];
                if (w >= 2 * k) H3[w] += c2 * H1[w - 2 * k];
                if (w == 3 * k) H3[w] += c3;
            }
            if (w >= k) H2[w] += c1 * H1[w - k];
            if (w == 2 * k) H2[w] += c2;
            if (w == k) H1[w] += c1;
        }
    }
}

static void dil(const seq_t *x, long long k, seq_t *out, long long N) {
    long long i;
    for (i = 0; i <= N; i++) out[i] = 0;
    for (i = 0; i * k <= N; i++) out[i * k] = x[i];
}

static int fixed_bits(void) {
    seq_t x = 1;
    int b = 0;
    while (b < 300) {
        x = x + x;
        b++;
        if (x == 0) return b;
    }
    return 0;
}

static seq_t from_u128(unsigned __int128 v) {
    seq_t two64 = 1;
    int i;
    for (i = 0; i < 64; i++) two64 = two64 + two64;
    return (seq_t)(uint64_t)(v >> 64) * two64 + (seq_t)(uint64_t)v;
}

static void mul(const seq_t *x, const seq_t *y, seq_t *c, long long N) {
    long long i, j;
    for (i = 0; i <= N; i++) c[i] = 0;
    for (i = 0; i <= N; i++) {
        if (x[i] == 0) continue;
        for (j = 0; i + j <= N; j++) c[i + j] += x[i] * y[j];
    }
}

static seq_t *zs(long long len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    long long i;
    for (i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *cache = NULL;
static long long cacheN = -1;

static void build(long long N) {
    long long L1 = N + 1, i, k, m, j;
    seq_t *T = zs(L1), *T2 = zs(L1), *P = zs(L1), *Ps = zs(L1), *U = zs(L1);
    seq_t *L = zs(L1), *E = zs(L1), *E2 = zs(L1), *Np = zs(L1), *Np2 = zs(L1);
    seq_t *Np3 = zs(L1), *h1 = zs(L1), *h2 = zs(L1), *h3 = zs(L1), *D2T = zs(L1);
    seq_t *tmp = zs(L1), *tmp2 = zs(L1), *R = zs(L1), *M = zs(L1), *W = zs(L1);

    /* rooted trees A000081 */
    if (fixed_bits()) {
        unsigned __int128 *tt = (unsigned __int128 *)calloc((size_t)L1, sizeof(unsigned __int128));
        unsigned __int128 *ss = (unsigned __int128 *)calloc((size_t)L1, sizeof(unsigned __int128));
        if (N >= 1) tt[1] = 1;
        for (m = 1; m < N; m++) {
            unsigned __int128 s = 0;
            for (j = m; j <= N; j += m) ss[j] += (unsigned __int128)m * tt[m];
            for (k = 1; k <= m; k++) s += ss[k] * tt[m - k + 1];
            tt[m + 1] = s / (unsigned __int128)m;
        }
        for (i = 0; i <= N; i++) T[i] = from_u128(tt[i]);
        free(tt); free(ss);
    } else {
        seq_t *ss = zs(L1);
        if (N >= 1) T[1] = 1;
        for (m = 1; m < N; m++) {
            seq_t s = 0;
            for (j = m; j <= N; j += m) ss[j] += T[m] * (seq_t)m;
            for (k = 1; k <= m; k++) s += ss[k] * T[m - k + 1];
            T[m + 1] = s / (seq_t)m;
        }
        mem_free(ss);
    }
    dil(T, 2, T2, N);

    P[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t s = 0;
        for (i = 1; i <= k; i++) s += T[i] * P[k - i];
        P[k] = s;
    }
    Ps[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t s = T[k];
        for (i = 2; i <= k; i += 2) s += T2[i] * Ps[k - i];
        Ps[k] = s;
    }
    choose3(T, N, 0, 0, h1, h2, h3);
    for (k = 0; k <= N; k++) D2T[k] = h2[k];
    mul(D2T, P, tmp, N);
    U[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t s = T[k] + tmp[k];
        for (i = 2; i <= k; i += 2) s += T2[i] * U[k - i];
        U[k] = s;
    }
    L[0] = 0;
    for (k = 1; k <= N; k++) L[k] = U[k] - T[k];

    /* figure-eight */
    choose3(L, N, 1, 0, h1, h2, h3);
    mul(T, h2, tmp, N);
    for (k = 0; k <= N; k++) R[k] += tmp[k];

    /* dumbbell */
    mul(T, L, E, N);
    dil(E, 2, E2, N);
    mul(E2, U, tmp, N);
    for (k = 0; k <= N; k++) R[k] += tmp[k];
    choose3(E, N, 0, 0, h1, h2, h3);
    mul(h2, P, tmp, N);
    for (k = 0; k <= N; k++) R[k] += tmp[k];

    /* theta, distinct end trees */
    choose3(P, N, 1, 1, h1, h2, h3);
    for (k = 0; k <= N; k++) M[k] = h3[k] - P[k];
    mul(D2T, M, tmp, N);
    for (k = 0; k <= N; k++) R[k] += tmp[k];

    /* theta, equal end trees: 3 paths up to global reversal */
    for (k = 0; k <= N; k++) Np[k] = U[k] - Ps[k];
    dil(Np, 2, Np2, N);
    dil(Np, 3, Np3, N);
    choose3(Ps, N, 1, 1, h1, h2, h3);
    for (k = 0; k <= N; k++) W[k] += h3[k];
    mul(h2, Np, tmp, N);
    for (k = 0; k <= N; k++) W[k] += tmp[k];
    choose3(Np, N, 0, 1, h1, h2, h3);
    for (k = 0; k <= N; k++) tmp2[k] = Np2[k] * 2 + h2[k] * 2;
    mul(Ps, tmp2, tmp, N);
    for (k = 0; k <= N; k++) W[k] += tmp[k];
    mul(Np2, Np, tmp, N);
    for (k = 0; k <= N; k++) {
        W[k] += tmp[k] * 3;
        W[k] -= Np3[k];
        W[k] += h3[k] * 4;
        W[k] -= U[k];
    }
    mul(T2, W, tmp, N);
    for (k = 0; k <= N; k++) R[k] += tmp[k];

    mem_free(T); mem_free(T2); mem_free(P); mem_free(Ps); mem_free(U); mem_free(L); mem_free(E); mem_free(E2);
    mem_free(Np); mem_free(Np2); mem_free(Np3); mem_free(h1); mem_free(h2); mem_free(h3); mem_free(D2T);
    mem_free(tmp); mem_free(tmp2); mem_free(M); mem_free(W);
    if (cache) mem_free(cache);
    cache = R;
    cacheN = N;
}

seq_t a(idx_t n) {
    long long nn = (long long)n;
    if (nn > cacheN) {
        long long N = nn;
        if (N < 2 * cacheN) N = 2 * cacheN;
        if (N < 40) N = 40;
        build(N);
    }
    return cache[nn];
}
