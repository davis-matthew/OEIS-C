/*
 * A005336  Number of labeled nonseparable (or 2-connected) bipartite graphs
 *          with 2n nodes.
 * https://oeis.org/A005336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let b(m) = Sum_{k=0..m} binomial(m,k)*2^(k*(m-k)) (2-colored
 *            bipartite graphs, A047863), c(m) = (b(m) - Sum_{k=1..m-1}
 *            binomial(m-1,k-1)*2*c(k)*b(m-k))/2 (connected bipartite,
 *            A001832). With R(x) = Sum_{j>=1} j*c(j) x^j/j! (rooted
 *            connected), blocks satisfy R(x) = x*exp(B'(R(x))), B'(x) =
 *            Sum_{k>...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   alloc_zero, pw2, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/pw2.h"

static seq_t *res = NULL;
static idx_t have = 0;

static void build(idx_t N)
{
    idx_t M = 2 * N, L = M + 1, K = M - 1, KL = K + 1;
    idx_t i, j, k, m, n;
    seq_t *C = alloc_zero(L * L);
    seq_t *b = alloc_zero(L), *c2 = alloc_zero(L), *c = alloc_zero(L);
    seq_t *P = alloc_zero(KL), *Q = alloc_zero(KL), *R = alloc_zero(KL), *beta = alloc_zero(KL);
    seq_t *Bt = alloc_zero(KL * KL);
    seq_t s;

    for (i = 0; i <= M; i++) {
        C[i * L] = 1;
        for (j = 1; j <= i; j++) {
            s = C[(i - 1) * L + j - 1];
            if (j < i) s += C[(i - 1) * L + j];
            C[i * L + j] = s;
        }
    }
    b[0] = 1;
    for (m = 1; m <= M; m++) {
        s = 0;
        for (k = 0; k <= m; k++) s += C[m * L + k] * pw2(k * (m - k));
        b[m] = s;
    }
    for (m = 1; m <= M; m++) {
        s = b[m];
        for (k = 1; k < m; k++) s -= C[(m - 1) * L + k - 1] * c2[k] * b[m - k];
        c2[m] = s;
        c[m] = s / 2;
    }
    for (j = 0; j <= K; j++) P[j] = c[j + 1];
    for (j = 1; j <= K; j++) {
        s = P[j];
        for (k = 1; k < j; k++) s -= C[(j - 1) * L + k - 1] * Q[k] * P[j - k];
        Q[j] = s;
        R[j] = (seq_t)j * c[j];
    }
    Bt[0] = 1;
    for (n = 1; n <= K; n++) {
        for (k = 1; k <= n; k++) {
            s = 0;
            for (j = 1; j + k <= n + 1; j++)
                s += C[(n - 1) * L + j - 1] * R[j] * Bt[(n - j) * KL + k - 1];
            Bt[n * KL + k] = s;
        }
    }
    for (n = 1; n <= K; n++) {
        s = Q[n];
        for (k = 1; k < n; k++) s -= beta[k] * Bt[n * KL + k];
        beta[n] = s;
    }

    if (res) release(res, have + 1);
    res = alloc_zero(N + 1);
    for (i = 1; i <= N; i++) res[i] = beta[2 * i - 1];
    have = N;

    release(C, L * L);
    release(b, L); release(c2, L); release(c, L);
    release(P, KL); release(Q, KL); release(R, KL); release(beta, KL);
    release(Bt, KL * KL);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > have) {
        idx_t t = have + have / 4 + 1;
        if (t < n) t = n;
        build(t);
    }
    return res[n];
}
