/*
 * A000677  Number of bicentered trees with n nodes.
 * https://oeis.org/A000677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{h>=0} ([x^n] R_h(x)^2 + [x^n] R_h(x^2)) / 2, where
 *            R_h(x) = A_h(x) - A_{h-1}(x) counts rooted trees of height
 *            exactly h, A_{-1} = 0, A_h(x) = x*Product_{k>=1}
 *            (1-x^k)^(-[x^k]A_{h-1}(x)) (height <= h). (Equivalently a(n) =
 *            A000055(n) - A000676(n): bicentered trees are two rooted
 *            trees...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 29, u64 n <= 51, u128 n <= 92,
 *            big any n
 * Helpers:   a677_from_u128, a677_u128_zero, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/a677_from_u128.h"
#include "../utils/a677_u128_zero.h"

#define A677_DEFINE(NAME, T, ALLOC)                                         \
static void NAME(long long N, T *out)                                       \
{                                                                           \
    T *P = ALLOC((idx_t)(N + 2));                                           \
    T *C = ALLOC((idx_t)(N + 2));                                           \
    T *S = ALLOC((idx_t)(N + 2));                                           \
    T *B = ALLOC((idx_t)(N + 2));                                           \
    T *R = ALLOC((idx_t)(N + 2));                                           \
    T *Q = ALLOC((idx_t)(N + 2));                                           \
    T *tmp;                                                                 \
    long long h, i, j, k, d, m, M;                                          \
    for (i = 0; i <= N + 1; i++) { P[i] = 0; Q[i] = 0; }                    \
    for (h = 0; 2 * (h + 1) <= N; h++) {                                    \
        M = N - h - 1;                                                      \
        for (k = 0; k <= M; k++) S[k] = 0;                                  \
        for (d = 1; d <= M - 1; d++)                                        \
            for (k = d; k <= M - 1; k += d)                                 \
                S[k] += P[d] * (idx_t)d;                                    \
        B[0] = 1;                                                           \
        for (m = 1; m <= M - 1; m++) {                                      \
            T t = 0;                                                        \
            for (k = 1; k <= m; k++) t += S[k] * B[m - k];                  \
            B[m] = t / (idx_t)m;                                            \
        }                                                                   \
        C[0] = 0;                                                           \
        for (m = 1; m <= M; m++) C[m] = B[m - 1];                           \
        for (i = 0; i <= M; i++) R[i] = C[i] - P[i];                        \
        for (i = h + 1; i <= M; i++) {                                      \
            if (2 * i <= N) Q[2 * i] += R[i];                               \
            for (j = h + 1; j <= M && i + j <= N; j++)                      \
                Q[i + j] += R[i] * R[j];                                    \
        }                                                                   \
        tmp = P; P = C; C = tmp;                                            \
        for (i = M + 1; i <= N + 1; i++) P[i] = 0;                          \
    }                                                                       \
    for (i = 0; i <= N; i++) out[i] = Q[i] / 2;                             \
}

A677_DEFINE(a677_compute_big, seq_t, alloc_zero)
A677_DEFINE(a677_compute_fix, unsigned __int128, a677_u128_zero)

static int a677_fixed_build(void)
{
    seq_t m = 0;
    m -= 1;
    return m > 0;
}

#define A677_FIX_CAP 200

seq_t a(idx_t n)
{
    static seq_t *memo = 0;
    static long long memoN = -1;
    int fixed = a677_fixed_build();
    if (fixed && n > A677_FIX_CAP) return 0;
    if ((long long)n > memoN) {
        long long N = (long long)n, i;
        if (N < memoN + memoN / 4) N = memoN + memoN / 4;
        if (N < 8) N = 8;
        if (fixed && N > A677_FIX_CAP) N = A677_FIX_CAP;
        seq_t *nm = alloc_zero((idx_t)(N + 1));
        if (fixed) {
            unsigned __int128 *tmp = a677_u128_zero((idx_t)(N + 1));
            a677_compute_fix(N, tmp);
            for (i = 0; i <= N; i++) nm[i] = a677_from_u128(tmp[i]);
            free(tmp);
        } else {
            a677_compute_big(N, nm);
        }
        memo = nm;
        memoN = N;
    }
    return memo[n];
}
