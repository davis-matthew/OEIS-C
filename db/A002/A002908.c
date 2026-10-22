/*
 * A002908  High temperature expansion of -u/J in odd powers of v =
 *          tanh(J/kT), where u is energy per site of the spin-1/2 Ising model
 *          on square lattice with nearest-neighbor interaction J at
 *          temperature T.
 * https://oeis.org/A002908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   with h(0) = 0, h(1) = 1 and, for N >= 2, (N-1)^2*h(N) =
 *            (6*(N-1)*(N-2)+2)*h(N-1) - (6*(N-3)*(N-2)+2)*h(N-3) +
 *            (N-3)^2*h(N-4), where h(N) = Sum_{m>=0} binomial(2m,m)^2 *
 *            binomial(N+2m,4m+1), a(n) = (5*h(n) + 5*h(n-1) - h(n+1) - h(n-2)
 *            + [n=1]) / 2. Equivalently, Sum_{n>=1} a(n) x^n = (1+x) * (
 *            2x/...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   A_grow, a2908_fixed, a2908_mulu, a2908_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/A_grow.h"
#include "../utils/a2908_fixed.h"
#include "../utils/a2908_mulu.h"
#include "../utils/a2908_prefix.h"

/* ---------- arbitrary precision: holonomic recurrence ---------- */
static seq_t *a2908_H = NULL;
static idx_t a2908_Hn = 0;

static void a2908_h_ensure(idx_t top) {
    idx_t newlen, N, start;
    if (top < a2908_Hn) return;
    newlen = top + 1;
    if (newlen < 2 * a2908_Hn) newlen = 2 * a2908_Hn;
    if (newlen < 2) newlen = 2;
    a2908_H = A_grow(a2908_H, a2908_Hn, newlen);
    if (a2908_Hn == 0) {
        a2908_H[0] = 0;
        a2908_H[1] = 1;
        a2908_Hn = 2;
    }
    start = a2908_Hn;
    for (N = start; N < newlen; N++) {
        seq_t v = (seq_t)(6 * (N - 1) * (N - 2) + 2) * a2908_H[N - 1];
        if (N >= 4) v += (seq_t)((N - 3) * (N - 3)) * a2908_H[N - 4];
        if (N >= 3) v -= (seq_t)(6 * (N - 3) * (N - 2) + 2) * a2908_H[N - 3];
        v /= (seq_t)((N - 1) * (N - 1));
        a2908_H[N] = v;
    }
    a2908_Hn = newlen;
}

static seq_t a2908_big(idx_t n) {
    seq_t v;
    a2908_h_ensure(n + 1);
    v = (seq_t)5 * (a2908_H[n] + a2908_H[n - 1]);
    v -= a2908_H[n + 1];
    if (n >= 2) v -= a2908_H[n - 2];
    if (n == 1) v += 1;
    return v / 2;
}

/* ---------- fixed width: generating function mod 2^w ---------- */
static seq_t *a2908_F = NULL;
static idx_t a2908_Fn = 0;

static void a2908_fixed_compute(idx_t L) {
    idx_t M = (L - 1) / 2, m, r, k;
    seq_t *c = (seq_t *)malloc((size_t)(M + 1) * sizeof(seq_t));
    seq_t *P = (seq_t *)malloc((size_t)(2 * M + 2) * sizeof(seq_t));
    seq_t *T = (seq_t *)malloc((size_t)L * sizeof(seq_t));
    seq_t *E = (seq_t *)malloc((size_t)L * sizeof(seq_t));

    for (k = 0; k < 2 * M + 2; k++) P[k] = 0;
    P[0] = 1;
    for (r = 1; r + 1 <= 2 * M; r++) {
        for (k = r; k >= 1; k--) P[k] += P[k - 1];
        if (r % 2 == 1) {
            m = (r + 1) / 2;
            c[m] = (seq_t)2 * P[m - 1] * P[m - 1];
        }
    }

    for (k = 0; k < L; k++) T[k] = 0;
    if (M >= 1) {
        T[0] = c[M];
        for (m = M - 1; m >= 1; m--) {
            a2908_mulu(T, L);
            T[0] += c[m];
        }
        a2908_mulu(T, L);
        a2908_prefix(T, L);
        a2908_prefix(T, L);
    }

    for (k = 0; k < L; k++) {
        seq_t e = (seq_t)(2 * k);
        e -= T[k];
        if (k >= 1) e += (seq_t)6 * T[k - 1];
        if (k >= 2) e -= T[k - 2];
        E[k] = e;
    }

    if (a2908_F) free(a2908_F);
    a2908_F = (seq_t *)malloc((size_t)L * sizeof(seq_t));
    a2908_F[0] = 0;
    for (k = 1; k < L; k++) a2908_F[k] = E[k] + E[k - 1];
    a2908_Fn = L;

    free(c);
    free(P);
    free(T);
    free(E);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a2908_fixed()) {
        if (n >= a2908_Fn) {
            idx_t L = n + 1;
            if (L < 2 * a2908_Fn) L = 2 * a2908_Fn;
            if (L < 8) L = 8;
            a2908_fixed_compute(L);
        }
        return a2908_F[n];
    }
    return a2908_big(n);
}
