/*
 * A006354  Expansion of a cusp form of weight 8 for Gamma_1(6).
 * https://oeis.org/A006354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |[q^n] (eta(z)^4*eta(2z)^4 - 9*eta(3z)^4*eta(6z)^4)^2|
 *            computed as a(n) = |[q^(n-1)] D(q)^2|, D(q) = A(q) - 9*q*A(q^3),
 *            A(q) = E(q)*E(q^2), E(q) = prod (1-q^k)^4 = (Euler pentagonal
 *            series)*(Jacobi series for eta^3)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/from_i128.h"

static __int128 *S_cache = NULL;
static idx_t S_cap = 0;

static void build(idx_t N) {
    long long *P = (long long *)calloc(N, sizeof(long long));
    long long *J = (long long *)calloc(N, sizeof(long long));
    long long *E = (long long *)calloc(N, sizeof(long long));
    long long *A = (long long *)calloc(N, sizeof(long long));
    long long *D = (long long *)calloc(N, sizeof(long long));
    idx_t *pi = (idx_t *)malloc(sizeof(idx_t) * (N + 2));
    idx_t *ji = (idx_t *)malloc(sizeof(idx_t) * (N + 2));
    idx_t np = 0, nj = 0, i, j, k;

    /* Euler: prod (1-q^k) = sum_k (-1)^k q^{k(3k-1)/2}, k over all integers */
    P[0] = 1;
    pi[np++] = 0;
    for (k = 1;; k++) {
        idx_t e1 = k * (3 * k - 1) / 2, e2 = k * (3 * k + 1) / 2;
        long long s = (k & 1) ? -1 : 1;
        if (e1 >= N) break;
        P[e1] = s; pi[np++] = e1;
        if (e2 < N) { P[e2] = s; pi[np++] = e2; }
    }
    /* Jacobi: prod (1-q^k)^3 = sum_{m>=0} (-1)^m (2m+1) q^{m(m+1)/2} */
    for (k = 0;; k++) {
        idx_t e = k * (k + 1) / 2;
        if (e >= N) break;
        J[e] = (k & 1) ? -(long long)(2 * k + 1) : (long long)(2 * k + 1);
        ji[nj++] = e;
    }
    for (i = 0; i < np; i++)
        for (j = 0; j < nj; j++) {
            idx_t e = pi[i] + ji[j];
            if (e < N) E[e] += P[pi[i]] * J[ji[j]];
        }
    /* A(q) = E(q) * E(q^2) */
    for (j = 0; 2 * j < N; j++) {
        long long c = E[j];
        if (c == 0) continue;
        for (i = 2 * j; i < N; i++) A[i] += E[i - 2 * j] * c;
    }
    /* D(q) = A(q) - 9 q A(q^3) */
    for (i = 0; i < N; i++) {
        D[i] = A[i];
        if (i >= 1 && (i - 1) % 3 == 0) D[i] -= 9 * A[(i - 1) / 3];
    }
    if (S_cache) free(S_cache);
    S_cache = (__int128 *)malloc(sizeof(__int128) * N);
    for (i = 0; i < N; i++) {
        __int128 s = 0;
        for (j = 0; j <= i; j++) s += (__int128)D[j] * D[i - j];
        S_cache[i] = s;
    }
    S_cap = N;
    free(P); free(J); free(E); free(A); free(D); free(pi); free(ji);
}

seq_t a(idx_t n) {
    __int128 v;
    if (n < 1) return 0;
    if (n > S_cap) {
        idx_t N = 2 * S_cap;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build(N);
    }
    v = S_cache[n - 1];
    if (v < 0) v = -v;
    return from_i128(v);
}
