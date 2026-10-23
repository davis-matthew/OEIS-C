/*
 * A048241  Number of colors that can be mixed with n >= 0 units of yellow,
 *          blue, red.
 * https://oeis.org/A048241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + Σ_{d=1}^{n} μ(d) * (C(⌊n/d⌋+3,3) - 1) where μ is the
 *            Möbius function and C(x,3)=x·(x-1)·(x-2)/6. This counts all
 *            non‑zero triples (i,j,k) with i+j+k ≤ n and gcd(i,j,k)=1, then
 *            adds the all‑zero triple
 * Range:     u8 n <= 10, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Compute the Möbius function μ(d) for 1 ≤ d ≤ N.
   The result array mu[1..N] contains -1,0,1 values. */
static void compute_mu(idx_t N, int *mu)
{
    if (N == 0) return;

    /* Use calloc to zero‑initialize the auxiliary arrays. */
    idx_t *lp  = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t));
    idx_t *pr  = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t));
    size_t pc  = 0;          /* number of primes found */

    mu[0] = 0;               /* unused */
    mu[1] = 1;

    for (idx_t i = 2; i <= N; ++i) {
        if (lp[i] == 0) {            /* i is prime */
            lp[i] = i;
            pr[pc++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = pr[j];
            if (p > lp[i] || i * p > N) break;
            idx_t v = i * p;
            lp[v] = p;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    free(lp);
    free(pr);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* only the null color */

    /* Allocate μ array and compute it. */
    int *mu = (int *)malloc((size_t)(n + 1) * sizeof(int));
    if (!mu) return (seq_t)0;      /* unlikely on typical systems */
    compute_mu(n, mu);

    /* Accumulate the Möbius weighted count in a signed 64‑bit variable. */
    int64_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int mu_val = mu[d];
        if (mu_val == 0) continue;

        idx_t m = n / d;                 /* ⌊n/d⌋ */
        /* Compute C(m+3,3) in 64‑bit arithmetic, safe for n ≤ 10000. */
        int64_t comb = ((int64_t)(m + 3) * (m + 2) * (m + 1)) / 6;
        int64_t term = comb - 1;         /* exclude the (0,0,0) triple */
        sum += mu_val * term;
    }

    free(mu);

    /* Add back the all‑zero triple. */
    return (seq_t)(sum + 1);
}
