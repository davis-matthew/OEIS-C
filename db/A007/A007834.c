/*
 * A007834  Number of point labeled reduced 5-free two-graphs with n nodes.
 * https://oeis.org/A007834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. sum_{n>=1} a(n) x^(n-1)/(n-1)! =
 *            -2*LambertW(-exp(-1/2)*sqrt(1+x)/2)/(1+x) = (1+y)/(1+x), where
 *            (1+y)^2*exp(-y) = 1+x, i.e. y' = exp(y)/(1-y^2), y(0) = 0. With
 *            egf coefficients y_n, E_n = [exp(y)]_n, S_n = [y^2]_n: E_0 = 1,
 *            E_{m+1} = Sum_{k=0..m} C(m,k) y_{k+1} E_{m-k}, S_n =
 *            Sum_{k=1..n-1...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *Acache = 0;
static idx_t Alen = 0; /* number of valid entries b_0..b_{Alen-1} */

static void build(idx_t M) /* compute b_0..b_M */
{
    idx_t L = M + 2, n, k, j;
    seq_t *y = alloc_zero(L);
    seq_t *E = alloc_zero(L);
    seq_t *S = alloc_zero(L);
    seq_t *row = alloc_zero(L);
    seq_t *b;

    y[0] = 0;
    E[0] = 1;
    S[0] = 0;
    y[1] = 1;
    row[0] = 1; /* row 0 of Pascal's triangle */
    for (n = 1; n < M; n++) {
        seq_t t;
        /* E_n using row n-1 */
        t = 0;
        for (k = 0; k <= n - 1; k++)
            t += row[k] * y[k + 1] * E[n - 1 - k];
        E[n] = t;
        /* advance row to row n */
        row[n] = 0;
        for (k = n; k >= 1; k--)
            row[k] += row[k - 1];
        /* S_n */
        t = 0;
        for (k = 1; k + 1 <= n; k++)
            t += row[k] * y[k] * y[n - k];
        S[n] = t;
        /* y_{n+1} */
        t = E[n];
        for (k = 2; k <= n; k++)
            t += row[k] * S[k] * y[n - k + 1];
        y[n + 1] = t;
    }

    b = alloc_zero(M + 1);
    b[0] = 1;
    for (j = 1; j <= M; j++)
        b[j] = y[j] - (seq_t)j * b[j - 1];

    release(y, L);
    release(E, L);
    release(S, L);
    release(row, L);
    if (Acache)
        release(Acache, Alen);
    Acache = b;
    Alen = M + 1;
}

seq_t a(idx_t n)
{
    idx_t m;
    if (n < 1)
        return 0;
    m = n - 1;
    if (m >= Alen) {
        idx_t M = m;
        if (M < 2 * Alen)
            M = 2 * Alen;
        if (M < 16)
            M = 16;
        build(M);
    }
    return Acache[m];
}
