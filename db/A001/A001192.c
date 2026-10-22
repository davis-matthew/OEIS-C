/*
 * A001192  Number of full sets of size n.
 * https://oeis.org/A001192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 1; from 1/(1-x) = Sum_{k>=0} a(k)*x^k*(1-x)^(2^k-k-1):
 *            a(m) = 1 - Sum_{k=0..m-1} a(k)*(-1)^(m-k)*C(2^k-k-1, m-k). The
 *            coefficients C(2^k-k-1, j) are the coefficients of Q_k =
 *            (1+x)^(2^k-k-1), truncated to degree n-k, built with Q_0 = 1 and
 *            Q_{k+1} = Q_k^2 * (1+x)^k
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   a001192_zalloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001192_zalloc.h"

static seq_t *a001192_memo = NULL;
static idx_t a001192_have = 0;

static void a001192_compute(idx_t N) {
    idx_t L = N + 1;
    seq_t *A = a001192_zalloc(L);
    seq_t *pos = a001192_zalloc(L);
    seq_t *neg = a001192_zalloc(L);
    seq_t *Q = a001192_zalloc(L);
    seq_t *T = a001192_zalloc(L);
    idx_t k, i, j, r;

    Q[0] = 1;
    for (k = 0; k <= N; k++) {
        seq_t ak;
        idx_t d = N - k;
        if (k == 0) {
            ak = 1;
        } else {
            ak = (neg[k] + 1) - pos[k];
        }
        A[k] = ak;

        for (j = 1; j <= d; j++) {
            seq_t t = ak * Q[j];
            if (j & 1) neg[k + j] += t;
            else pos[k + j] += t;
        }
        if (k == N) break;

        /* Q_{k+1} = Q_k^2 * (1+x)^k, truncated to degree d-1 */
        {
            idx_t e = d - 1;
            for (i = 0; i <= e; i++) T[i] = 0;
            for (i = 0; i <= e; i++) {
                for (j = 0; i + j <= e; j++) {
                    T[i + j] += Q[i] * Q[j];
                }
            }
            for (r = 0; r < k; r++) {
                for (i = e; i >= 1; i--) {
                    T[i] += T[i - 1];
                }
            }
            for (i = 0; i <= e; i++) Q[i] = T[i];
        }
    }

    free(pos);
    free(neg);
    free(Q);
    free(T);
    if (a001192_memo != NULL) free(a001192_memo);
    a001192_memo = A;
    a001192_have = L;
}

seq_t a(idx_t n) {
    if (n >= a001192_have) {
        idx_t target = n + n / 4 + 4;
        a001192_compute(target);
    }
    return a001192_memo[n];
}
