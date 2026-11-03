/*
 * A006136  T(n+3,3) from table A045912 of characteristic polynomial of
 *          negative Pascal matrix.
 * https://oeis.org/A006136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e_3(P_{n+3}), the sum of all 3x3 principal minors of the
 *            symmetric Pascal matrix P_N = [C(i+j,i)]_{0<=i,j<N}, N = n+3.
 *            Via Newton: a(n) = (p1^3 - 3*p1*p2 + 2*p3)/6, p_k =
 *            trace(P_N^k), where G = P_N^2 satisfies G(0,j) = C(j+N,j+1),
 *            G(i,j) = G(i-1,j) - G(i-1,j+1) + C(i+N-1,i)*C(j+N,j+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

seq_t a(idx_t n) {
    idx_t N = n + 3;
    if (seq_is_fixed_width()) {
        /* a(n) >= 16^n, so only n <= bits/4 can fit; sum minors mod 2^bits */
        idx_t w = (idx_t)(8 * sizeof(seq_t));
        if (n > w / 4) return 0;
        seq_t P[40][40];
        idx_t i, j, k;
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                P[i][j] = (i == 0 || j == 0) ? (seq_t)1 : (seq_t)(P[i-1][j] + P[i][j-1]);
        seq_t s = 0;
        for (i = 0; i < N; i++)
            for (j = i + 1; j < N; j++)
                for (k = j + 1; k < N; k++) {
                    seq_t pos = (seq_t)(P[i][i] * P[j][j] * P[k][k]);
                    pos += (seq_t)(2 * P[i][j] * P[j][k] * P[i][k]);
                    seq_t neg = (seq_t)(P[i][i] * P[j][k] * P[j][k]);
                    neg += (seq_t)(P[j][j] * P[i][k] * P[i][k]);
                    neg += (seq_t)(P[k][k] * P[i][j] * P[i][j]);
                    s += pos;
                    s -= neg;
                }
        return s;
    }

    idx_t L = 2 * N - 1;           /* G row entries j = 0..2N-2 */
    seq_t *G = alloc_zero(L);
    seq_t *B = alloc_zero(L);      /* B[j] = C(j+N, j+1) */
    seq_t *R = alloc_zero(N);      /* R[j] = C(i+j, i) for current row i */
    idx_t i, j;
    B[0] = (seq_t)N;
    for (j = 1; j < L; j++) {
        B[j] = B[j-1] * (seq_t)(j + N);
        B[j] /= (seq_t)(j + 1);
    }
    for (j = 0; j < L; j++) G[j] = B[j];
    for (j = 0; j < N; j++) R[j] = 1;

    seq_t c = 1, p1 = 0, p2 = 0, p3 = 0;
    for (i = 0; i < N; i++) {
        if (i > 0) {
            c = c * (seq_t)(i + N - 1);
            c /= (seq_t)i;
            idx_t len = L - i;
            for (j = 0; j < len; j++) {
                seq_t t = c * B[j];
                t += G[j];
                t -= G[j+1];
                G[j] = t;
            }
            for (j = 1; j < N; j++) R[j] += R[j-1];
        }
        p1 += R[i];
        p2 += G[i];
        for (j = 0; j < N; j++) p3 += G[j] * R[j];
    }

    seq_t res = p1 * p1 * p1;
    res += (seq_t)2 * p3;
    res -= (seq_t)3 * p1 * p2;
    res /= (seq_t)6;

    release(G, L);
    release(B, L);
    release(R, N);
    return res;
}
