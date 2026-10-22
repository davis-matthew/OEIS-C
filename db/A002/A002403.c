/*
 * A002403  Coefficients for step-by-step integration.
 * https://oeis.org/A002403
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 * lcm(1..n+1) * Integral_{t=0..1}
 *            t*(1-t)*Product_{m=2..n-1} (t+m) dt (negated Adams-Moulton
 *            coefficient of f_{-1} in the (n+1)-point formula, scaled by
 *            n!*lcm(1..n+1)). Writing Product_{m=2..n-1}(t+m) = Sum_k q_k
 *            t^k, this is a(n) = n*(n-1)/2 * Sum_{k=0..n-2} q_k * (L/(k+2) -
 *            L/...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define A002403_MAX 20002
static seq_t a002403_q[A002403_MAX];

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t N = n + 1;

    /* L = lcm(1..n+1) as a product of maximal prime powers */
    seq_t L = 1;
    bool *comp = (bool *)calloc((size_t)N + 1, sizeof(bool));
    for (idx_t p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (idx_t j = p * p; j <= N; j += p) comp[j] = true;
        idx_t pk = p;
        while (pk <= N / p) pk *= p;
        L *= (seq_t)pk;
    }
    free(comp);

    /* coefficients of Product_{m=2..n-1} (t+m) */
    idx_t deg = n - 2;
    seq_t *q = a002403_q;
    q[0] = 1;
    for (idx_t k = 1; k <= deg; k++) q[k] = 0;
    for (idx_t m = 2; m + 1 <= n; m++) {
        for (idx_t k = m - 1; k >= 1; k--)
            q[k] = q[k] * (seq_t)m + q[k - 1];
        q[0] = q[0] * (seq_t)m;
    }

    seq_t S = 0;
    for (idx_t k = 0; k <= deg; k++)
        S += q[k] * (L / (seq_t)(k + 2) - L / (seq_t)(k + 3));

    if (n % 2 == 0)
        return S * (seq_t)(n / 2) * (seq_t)(n - 1);
    return S * (seq_t)n * (seq_t)((n - 1) / 2);
}
