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
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static idx_t g2611_eta3_list(idx_t s, idx_t M, idx_t *e, long long *c)
{
    idx_t cnt = 0, m;
    for (m = 0; s * (m * (m + 1) / 2) < M; m++) {
        e[cnt] = s * (m * (m + 1) / 2);
        c[cnt] = ((m & 1) ? -1 : 1) * (long long)(2 * m + 1);
        cnt++;
    }
    return cnt;
}

static idx_t g2611_eta_list(idx_t s, idx_t M, idx_t *e, long long *c)
{
    idx_t cnt = 0, k;
    e[cnt] = 0; c[cnt] = 1; cnt++;
    for (k = 1;; k++) {
        idx_t p1 = s * (k * (3 * k - 1) / 2);
        idx_t p2 = s * (k * (3 * k + 1) / 2);
        long long sg = (k & 1) ? -1 : 1;
        if (p1 >= M) break;
        e[cnt] = p1; c[cnt] = sg; cnt++;
        if (p2 < M) { e[cnt] = p2; c[cnt] = sg; cnt++; }
    }
    return cnt;
}

static void g2611_mul_sparse(seq_t *F, idx_t M, const idx_t *e, const long long *c, idx_t cnt)
{
    idx_t j = M;
    while (j-- > 0) {
        seq_t acc = 0;
        idx_t i;
        for (i = 0; i < cnt && e[i] <= j; i++)
            acc += F[j - e[i]] * (seq_t)c[i];
        F[j] = acc;
    }
}

static long long g2611_r2(idx_t m)
{
    long long cnt = 0;
    idx_t d;
    if (m == 0) return 1;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            idx_t d2 = m / d;
            if (d % 4 == 1) cnt++; else if (d % 4 == 3) cnt--;
            if (d2 != d) {
                if (d2 % 4 == 1) cnt++; else if (d2 % 4 == 3) cnt--;
            }
        }
    }
    return 4 * cnt;
}

static seq_t *g2611_B = NULL;   /* g2611_B[i] = coeff of q^i in Prod (1-q^j)^8 (1-q^2j)^8 */
static idx_t g2611_Bn = 0;

static void g2611_build(idx_t M)
{
    seq_t *F = mem_new(seq_t, (size_t)M);
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
    if (g2611_B) mem_free(g2611_B);
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
