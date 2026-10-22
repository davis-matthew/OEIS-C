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
 * Helpers:   bin2, bin3, choose3, dil, fixed_bits, from_u128, mul, zs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bin2.h"
#include "../utils/bin3.h"
#include "../utils/choose3.h"
#include "../utils/dil.h"
#include "../utils/fixed_bits.h"
#include "../utils/from_u128.h"
#include "../utils/mul.h"
#include "../utils/zs.h"

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
        free(ss);
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

    free(T); free(T2); free(P); free(Ps); free(U); free(L); free(E); free(E2);
    free(Np); free(Np2); free(Np3); free(h1); free(h2); free(h3); free(D2T);
    free(tmp); free(tmp2); free(M); free(W);
    if (cache) free(cache);
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
