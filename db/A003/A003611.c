/*
 * A003611  Symmetries in planted 3-trees on n+1 vertices.
 * https://oeis.org/A003611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over rooted trees T on n nodes with every node having
 *            at most 2 children (planted 3-trees) of |Aut(T)|. With S_k(m) =
 *            sum of |Aut|^k over such trees on m nodes: S_k(1) = 1, S_k(m) =
 *            S_k(m-1) + Sum_{i<j, i+j=m-1} S_k(i)*S_k(j) + [m-1 = 2h] * (
 *            (S_k(h)^2 - S_2k(h))/2 + 2^k * S_2k(h) ), a...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   a3611_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a3611_mul.h"

#define A3611_MAXLEV 80

static seq_t *a3611_L[A3611_MAXLEV];
static idx_t a3611_M[A3611_MAXLEV];
static int a3611_nlev = 0;
static idx_t a3611_built = 0;

static void a3611_build(idx_t N)
{
    int j;
    idx_t M, m, i;
    seq_t pk[A3611_MAXLEV];

    for (j = 0; j < a3611_nlev; j++) free(a3611_L[j]);
    a3611_nlev = 0;

    M = N;
    for (;;) {
        a3611_M[a3611_nlev++] = M;
        if (M < 3) break;
        M = (M - 1) / 2;
    }
    pk[0] = 2;
    for (j = 1; j < a3611_nlev; j++) pk[j] = a3611_mul(pk[j - 1], pk[j - 1]);

    for (j = 0; j < a3611_nlev; j++) {
        idx_t len = a3611_M[j] + 1;
        a3611_L[j] = (seq_t *)malloc((size_t)len * sizeof(seq_t));
        for (i = 0; i < len; i++) a3611_L[j][i] = 0;
    }

    for (j = a3611_nlev - 1; j >= 0; j--) {
        seq_t *S = a3611_L[j];
        S[1] = 1;
        for (m = 2; m <= a3611_M[j]; m++) {
            idx_t t = m - 1;
            seq_t v = S[m - 1];
            for (i = 1; 2 * i < t; i++) v += a3611_mul(S[i], S[t - i]);
            if (t % 2 == 0) {
                idx_t h = t / 2;
                seq_t s = S[h];
                seq_t s2 = a3611_L[j + 1][h];
                seq_t q = s / 2, r = s % 2, q2 = s2 / 2;
                seq_t qq = q + q;
                seq_t qr = q + r;
                seq_t part = a3611_mul(qq, qr);
                part -= q2;              /* (s^2 - s2)/2, exact */
                v += part;
                v += a3611_mul(pk[j], s2);
            }
            S[m] = v;
        }
    }
    a3611_built = N;
}

seq_t a(idx_t n)
{
    if (n > a3611_built) {
        idx_t N = n;
        if (N < 64) N = 64;
        if (a3611_built > 0 && N < 2 * a3611_built) N = 2 * a3611_built;
        a3611_build(N);
    }
    return a3611_L[0][n];
}
