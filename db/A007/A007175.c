/*
 * A007175  Number of simplicial 4-clusters with n cells.
 * https://oeis.org/A007175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   chirally distinct (rotations only) trees of 4-simplices glued
 *            along facets, counted by cycle index + dissymmetry. A(x) = 1 +
 *            x*A(x)^4, C(x) = 1 + x*A(x^2)^2, D(x) = 1 + x*D(x)*A(x^3), G =
 *            A^5 + 15*C*A(x^2)^2 + 20*D^2*A(x^3) + 24*A(x^5), H = (A-1)^2 +
 *            3*(C-1)^2 + 8*(D-1)^2, a(n) = ( 2*[x^(n-1)]G -...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

#define A007175_SERIES(T, NAME, ALLOC, FREE)                                   \
static void NAME(idx_t M, T *P, T *Q)                                          \
{                                                                              \
    idx_t L = M + 1, i, k, m;                                                  \
    T *A = ALLOC(L), *A2 = ALLOC(L), *A4 = ALLOC(L);                           \
    T *C = ALLOC(L), *D = ALLOC(L), *D2 = ALLOC(L);                            \
    A[0] = 1; A2[0] = 1; A4[0] = 1;                                            \
    for (m = 1; m <= M; m++) {                                                 \
        T s = 0;                                                               \
        A[m] = A4[m - 1];                                                      \
        for (i = 0; i <= m; i++) s += A[i] * A[m - i];                         \
        A2[m] = s;                                                             \
        s = 0;                                                                 \
        for (i = 0; i <= m; i++) s += A2[i] * A2[m - i];                       \
        A4[m] = s;                                                             \
    }                                                                          \
    C[0] = 1; D[0] = 1;                                                        \
    for (m = 1; m <= M; m++) {                                                 \
        T s = 0;                                                               \
        C[m] = ((m - 1) % 2 == 0) ? A2[(m - 1) / 2] : (T)0;                    \
        for (k = 0; 3 * k <= m - 1; k++) s += A[k] * D[m - 1 - 3 * k];         \
        D[m] = s;                                                              \
    }                                                                          \
    for (m = 0; m <= M; m++) {                                                 \
        T s = 0;                                                               \
        for (i = 0; i <= m; i++) s += D[i] * D[m - i];                         \
        D2[m] = s;                                                             \
    }                                                                          \
    for (m = 1; m <= M; m++) {                                                 \
        idx_t g = m - 1;                                                       \
        T a5 = 0, ca = 0, da = 0, x5 = 0, h = 0, p;                            \
        for (i = 0; i <= g; i++) a5 += A4[i] * A[g - i];                       \
        for (k = 0; 2 * k <= g; k++) ca += A2[k] * C[g - 2 * k];               \
        for (k = 0; 3 * k <= g; k++) da += A[k] * D2[g - 3 * k];               \
        if (g % 5 == 0) x5 = A[g / 5];                                         \
        p = 2 * (a5 + 15 * ca + 20 * da + 24 * x5);                            \
        if (m % 2 == 0) p += 30 * (A[m / 2] + C[m / 2]);                       \
        for (i = 1; i < m; i++)                                                \
            h += A[i] * A[m - i] + 3 * (C[i] * C[m - i])                       \
                 + 8 * (D[i] * D[m - i]);                                      \
        P[m] = p;                                                              \
        Q[m] = 5 * h;                                                          \
    }                                                                          \
    FREE(A, L); FREE(A2, L); FREE(A4, L);                                      \
    FREE(C, L); FREE(D, L); FREE(D2, L);                                       \
}

typedef unsigned __int128 a007175_u128;

static a007175_u128 *a007175_ualloc(idx_t len)
{
    return (a007175_u128 *)calloc((size_t)len, sizeof(a007175_u128));
}

static void a007175_ufree(a007175_u128 *p, idx_t len)
{
    (void)len;
    free(p);
}

A007175_SERIES(a007175_u128, a007175_series_u, a007175_ualloc, a007175_ufree)
A007175_SERIES(seq_t, a007175_series_s, alloc_zero, release)

/* numerator N = 120*a(n) known mod 2^128: recover a(n) mod 2^125 */
static seq_t a007175_fix(a007175_u128 p, a007175_u128 q)
{
    a007175_u128 x = 15, r;
    int it;
    for (it = 0; it < 6; it++) x *= (a007175_u128)2 - (a007175_u128)15 * x;
    r = ((p - q) >> 3) * x;
    r &= (((a007175_u128)1) << 125) - 1;
    return u128_to_seq(r);
}

static seq_t *a007175_cache = 0;
static idx_t a007175_cap = 0;

seq_t a(idx_t n)
{
    if (n > a007175_cap || a007175_cache == 0) {
        idx_t M = n, m;
        seq_t *nc;
        if (M < a007175_cap + a007175_cap / 2) M = a007175_cap + a007175_cap / 2;
        if (M < 32) M = 32;
        nc = alloc_zero(M + 1);
        if (seq_is_fixed_width()) {
            a007175_u128 *P = a007175_ualloc(M + 1), *Q = a007175_ualloc(M + 1);
            a007175_series_u(M, P, Q);
            for (m = 1; m <= M; m++) nc[m] = a007175_fix(P[m], Q[m]);
            free(P);
            free(Q);
        } else {
            seq_t *P = alloc_zero(M + 1), *Q = alloc_zero(M + 1);
            a007175_series_s(M, P, Q);
            for (m = 1; m <= M; m++) nc[m] = (P[m] - Q[m]) / 120;
            release(P, M + 1);
            release(Q, M + 1);
        }
        if (a007175_cache) release(a007175_cache, a007175_cap + 1);
        a007175_cache = nc;
        a007175_cap = M;
    }
    return a007175_cache[n];
}
