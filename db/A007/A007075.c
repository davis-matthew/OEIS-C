/*
 * A007075  Number of irreducible positions of size n in Montreal solitaire.
 * https://oeis.org/A007075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] f(1), where f(v) = v*(1 + f(1+x*v) - f(1)). Writing
 *            f = Sum_k x^k Sum_m p(k,m) v^m (p(k,m) = indecomposable
 *            permutations with k inversions and m left-to-right minima):
 *            p(0,1) = 1, and for k >= 1: p(k,i+1) = Sum_m
 *            p(k-i,m)*binomial(m,i) for i >= 1. a(n) = Sum_m p(n,m). Since
 *            p(k,m) =...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];
    idx_t N = n;
    int *Mj = (int *)malloc((size_t)(N + 1) * sizeof(int));
    {
        long long m = 1;
        for (idx_t j = 0; j <= N; j++) {
            while ((unsigned long long)((m + 1) * m / 2) <= (unsigned long long)j) m++;
            Mj[j] = (int)m;
        }
    }
    int W = Mj[N] + 2;
    idx_t CW = (idx_t)(W + 1);
    seq_t *C = alloc_zero(CW * CW);
    C[0] = (seq_t)1;
    for (int m = 1; m <= W; m++) {
        C[(idx_t)m * CW] = (seq_t)1;
        for (int i = 1; i <= m; i++)
            C[(idx_t)m * CW + i] = C[(idx_t)(m - 1) * CW + (i - 1)] + C[(idx_t)(m - 1) * CW + i];
    }
    seq_t *P = alloc_zero((idx_t)W * CW);
    seq_t *res = alloc_zero(N + 1);
    for (idx_t k = 0; k <= N; k++) {
        seq_t *row = P + (idx_t)(k % (idx_t)W) * CW;
        for (int t = 0; t <= W; t++) row[t] = (seq_t)0;
        if (k == 0) {
            row[1] = (seq_t)1;
        } else {
            for (idx_t i = 1; i <= k; i++) {
                idx_t j = k - i;
                if ((long long)i > (long long)Mj[j]) break;
                seq_t *pj = P + (idx_t)(j % (idx_t)W) * CW;
                seq_t s = (seq_t)0;
                for (int m = (int)i; m <= Mj[j]; m++)
                    s += pj[m] * C[(idx_t)m * CW + i];
                row[i + 1] = s;
            }
        }
        seq_t tot = (seq_t)0;
        for (int t = 1; t <= W; t++) tot += row[t];
        res[k] = tot;
    }
    release(C, CW * CW);
    release(P, (idx_t)W * CW);
    free(Mj);
    if (memo) release(memo, memo_len);
    memo = res;
    memo_len = N + 1;
    return memo[n];
}
