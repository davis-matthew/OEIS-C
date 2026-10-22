/*
 * A006801  Number of 2-dimensional directed compact animals of size n.
 * https://oeis.org/A006801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Sum over width sequences (w_1,...,w_k) with w_1 = 1,
 *            w_1+...+w_k = n and 1 <= w_{i+1} <= w_i + 1, of Prod_i (w_i + 2
 *            - w_{i+1}). Each diagonal of the animal is one interval of
 *            cells; the next diagonal's interval [a',b'] must satisfy a <= a'
 *            <= b' <= b+1. Computed by DP: f(m,w') = Sum_{w >=...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *ans_memo = 0;
static idx_t ans_cnt = 0;

seq_t a(idx_t n)
{
    idx_t W, L, R, m, w;
    long long j;
    seq_t *V, *g, *U, *na;

    if (n <= ans_cnt) return ans_memo[n];

    W = 1;
    while ((W + 1) * (W + 2) / 2 <= n) W++;
    L = W + 2;
    R = W + 1;

    V = alloc_zero(R * L);
    g = alloc_zero(L);
    U = alloc_zero(L);
    na = alloc_zero(n + 1);

    V[0] = 1; /* row 0: V_0[0] = 1, others 0 */

    for (m = 1; m <= n; m++) {
        idx_t r = m % R;
        seq_t *row;
        for (w = 0; w < L; w++) g[w] = 0;
        for (w = 1; w <= W && w <= m; w++)
            g[w] = V[((m - w) % R) * L + (w - 1)];
        U[W + 1] = g[W + 1];
        for (j = (long long)W; j >= 0; j--) U[j] = U[j + 1] + g[j];
        row = V + r * L;
        row[W + 1] = U[W + 1];
        for (j = (long long)W; j >= 0; j--) row[j] = row[j + 1] + U[j];
        na[m] = U[1];
    }

    release(V, R * L);
    release(g, L);
    release(U, L);
    if (ans_memo) release(ans_memo, ans_cnt + 1);
    ans_memo = na;
    ans_cnt = n;
    return ans_memo[n];
}
