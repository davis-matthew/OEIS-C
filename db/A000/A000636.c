/*
 * A000636  Number of paraffins C_n H_{2n} X_2 with n carbon atoms.
 * https://oeis.org/A000636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: (1/2) * (1/(1-x*R(x)) + (1+x*R(x)) / (1-x^2*R(x^2))), R(x)
 *            = (T(x)^2 + T(x^2))/2 (g.f. of A000642), where T(x) is the g.f.
 *            of A000598: T(x) = 1 + x*(T(x)^3 + 3*T(x)*T(x^2) + 2*T(x^3))/6 =
 *            1 + x*(T(x)*R(x) + T(x)*T(x^2) + T(x^3))/3, T(0) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   grow_mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/grow_mem.h"

#define DEF_CORE(NAME, T)                                                   \
static T NAME(idx_t n) {                                                    \
    static T *A = NULL, *B = NULL, *R = NULL, *S = NULL, *Q = NULL,         \
             *P = NULL, *V = NULL;                                          \
    static idx_t len = 0, cap = 0;                                          \
    if (n + 2 >= cap) {                                                     \
        idx_t nc = (cap * 2 > n + 16) ? cap * 2 : n + 16;                   \
        A = (T *)grow_mem(A, cap, nc, sizeof(T));                           \
        B = (T *)grow_mem(B, cap, nc, sizeof(T));                           \
        R = (T *)grow_mem(R, cap, nc, sizeof(T));                           \
        S = (T *)grow_mem(S, cap, nc, sizeof(T));                           \
        Q = (T *)grow_mem(Q, cap, nc, sizeof(T));                           \
        P = (T *)grow_mem(P, cap, nc, sizeof(T));                           \
        V = (T *)grow_mem(V, cap, nc, sizeof(T));                           \
        cap = nc;                                                           \
    }                                                                       \
    if (len == 0) A[0] = 1;                                                 \
    while (len <= n) {                                                      \
        idx_t m = len, i, k;                                                \
        T s = 0;                                                            \
        for (i = 0; i <= m; i++) s += A[i] * A[m - i];                      \
        B[m] = s;                                                           \
        if (m % 2 == 0) s += A[m / 2];                                      \
        R[m] = s / 2;                                                       \
        s = 0;                                                              \
        for (i = 0; i <= m; i++) s += A[i] * R[m - i];                      \
        for (i = 0; 2 * i <= m; i++) s += A[i] * A[m - 2 * i];              \
        if (m % 3 == 0) s += A[m / 3];                                      \
        A[m + 1] = s / 3;                                                   \
        if (m == 0) {                                                       \
            S[0] = 1; Q[0] = 1; P[0] = 1;                                   \
        } else {                                                            \
            s = 0;                                                          \
            for (k = 1; k <= m; k++) s += R[k - 1] * S[m - k];              \
            S[m] = s;                                                       \
            s = 0;                                                          \
            for (k = 1; 2 * k <= m; k++) s += R[k - 1] * Q[m - 2 * k];      \
            Q[m] = s;                                                       \
            s = Q[m];                                                       \
            for (k = 1; k <= m; k++) s += R[k - 1] * Q[m - k];              \
            P[m] = s;                                                       \
        }                                                                   \
        V[m] = (S[m] + P[m]) / 2;                                           \
        len++;                                                              \
    }                                                                       \
    return V[n];                                                            \
}

DEF_CORE(core_wide, unsigned __int128)
DEF_CORE(core_seq, seq_t)

seq_t a(idx_t n) {
    if ((seq_t)((seq_t)0 - 1) > 0 && sizeof(seq_t) <= 8)
        return (seq_t)(uint64_t)core_wide(n);
    return core_seq(n);
}
