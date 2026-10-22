/*
 * A002611  Glaisher's function V(n).
 * https://oeis.org/A002611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} A004018(n-2*k) * A002288(k), where
 *            A004018(0) = 1, A004018(m) = 4*Sum_{d|m} chi_{-4}(d), and
 *            A002288(k) is the coefficient of q^k in q*Prod_{j>=1} (1-q^j)^8
 *            * (1-q^(2j))^8 = eta(z)^8*eta(2z)^8, built from sparse series
 *            eta = Sum (-1)^k q^(k(3k-1)/2) and eta^3 = Sum_{m...
 * Range:     i8 n <= 8, i16 n <= 32, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   g2611_eta3_list, g2611_eta_list, g2611_mul_sparse, g2611_r2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/g2611_eta3_list.h"
#include "../utils/g2611_eta_list.h"
#include "../utils/g2611_mul_sparse.h"
#include "../utils/g2611_r2.h"

static seq_t *g2611_B = NULL;   /* g2611_B[i] = coeff of q^i in Prod (1-q^j)^8 (1-q^2j)^8 */
static idx_t g2611_Bn = 0;

static void g2611_build(idx_t M)
{
    seq_t *F = (seq_t *)malloc((size_t)M * sizeof(seq_t));
    idx_t cap = 8;
    idx_t *e;
    long long *c;
    idx_t i, cnt;
    int s, r;
    while (cap * cap < 4 * M + 16) cap *= 2;
    cap = 2 * cap + 8;
    e = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    c = (long long *)malloc((size_t)cap * sizeof(long long));
    for (i = 0; i < M; i++) F[i] = 0;
    F[0] = 1;
    for (s = 1; s <= 2; s++) {
        cnt = g2611_eta3_list((idx_t)s, M, e, c);
        for (r = 0; r < 2; r++) g2611_mul_sparse(F, M, e, c, cnt);
        cnt = g2611_eta_list((idx_t)s, M, e, c);
        for (r = 0; r < 2; r++) g2611_mul_sparse(F, M, e, c, cnt);
    }
    free(e);
    free(c);
    if (g2611_B) free(g2611_B);
    g2611_B = F;
    g2611_Bn = M;
}

seq_t a(idx_t n)
{
    idx_t N = n / 2, k;
    seq_t result = 0;
    if (N == 0) return 0;
    if (g2611_Bn < N) {
        idx_t M = 2 * g2611_Bn;
        if (M < N) M = N;
        g2611_build(M);
    }
    for (k = 1; k <= N; k++) {
        long long r = g2611_r2(n - 2 * k);
        if (r != 0) result += g2611_B[k - 1] * (seq_t)r;
    }
    return result;
}
