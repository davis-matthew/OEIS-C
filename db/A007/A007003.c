/*
 * A007003  Euler transform of numbers of preferential arrangements.
 * https://oeis.org/A007003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n+1), where c is the Euler transform of b(k) =
 *            A000670(k-1): Prod_{k>=1} 1/(1-x^k)^b(k) = Sum c(m) x^m, n*c(n)
 *            = Sum_{k=1..n} (Sum_{d|k} d*b(d)) * c(n-k), c(0) = 1; A000670(m)
 *            = Sum_{k=1..m} binomial(m,k)*A000670(m-k), A000670(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a7003_gcd, alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/a7003_gcd.h"

/* exact c*t/j, given that j divides c*t, without overflowing the result */
static seq_t a7003_muldiv(seq_t c, seq_t t, idx_t j)
{
    if (seq_is_big())
        return c * t / j;
    seq_t sj = (seq_t)j;
    if (sj == 0)
        return c * t;
    seq_t g = a7003_gcd(c, sj);
    if (g == 0)
        g = 1;
    seq_t q = sj / g;
    return (c / g) * (t / q);
}

/* F[0..K-1] = Fubini numbers */
static void a7003_fubini(seq_t *F, idx_t K)
{
    idx_t m, k;
    F[0] = 1;
    for (m = 1; m < K; m++) {
        seq_t s = 0, C = 1;
        for (k = 1; k <= m; k++) {
            C = a7003_muldiv(C, (seq_t)(m - k + 1), k);
            s += C * F[m - k];
        }
        F[m] = s;
    }
}

static seq_t *a7003_memo = NULL;
static idx_t a7003_len = 0;

seq_t a(idx_t n)
{
    idx_t M = n + 1;
    if (!seq_is_big()) {
        /* fixed width: a(n) exceeds 128 bits well before n = 60 */
        if (n > 60)
            return 0;
        seq_t *F = alloc_zero(M);
        seq_t *P = alloc_zero(M + 1);
        seq_t *Cj = alloc_zero(M + 1);
        idx_t k, i, j;
        seq_t res;
        a7003_fubini(F, M);
        P[0] = 1;
        for (k = 1; k <= M; k++) {
            seq_t m = F[k - 1];
            idx_t J = M / k;
            Cj[0] = 1;
            for (j = 1; j <= J; j++)
                Cj[j] = a7003_muldiv(Cj[j - 1], m + (seq_t)(j - 1), j);
            for (i = M; i >= k; i--) {
                seq_t s = 0;
                for (j = 1; j * k <= i; j++)
                    s += Cj[j] * P[i - j * k];
                P[i] += s;
            }
        }
        res = P[M];
        release(F, M);
        release(P, M + 1);
        release(Cj, M + 1);
        return res;
    }

    if (M + 1 > a7003_len) {
        idx_t L = M + 1;
        idx_t grow = a7003_len + a7003_len / 2;
        if (grow > L)
            L = grow;
        idx_t K = L - 1;
        seq_t *F = alloc_zero(K);
        seq_t *ds = alloc_zero(L);
        seq_t *c = alloc_zero(L);
        idx_t d, k, i;
        a7003_fubini(F, K);
        for (d = 1; d <= K; d++) {
            seq_t t = F[d - 1] * d;
            for (k = d; k <= K; k += d)
                ds[k] += t;
        }
        c[0] = 1;
        for (i = 1; i <= K; i++) {
            seq_t s = 0;
            for (k = 1; k <= i; k++)
                s += ds[k] * c[i - k];
            c[i] = s / i;
        }
        release(F, K);
        release(ds, L);
        if (a7003_memo != NULL)
            release(a7003_memo, a7003_len);
        a7003_memo = c;
        a7003_len = L;
    }
    return a7003_memo[M];
}
