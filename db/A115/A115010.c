/*
 * A115010  Array read by antidiagonals: let V(m,n) = Sum_{i=1..m, j=1..n,
 *          gcd(i,j)=1} (m+1-i)*(n+1-j), then T(m,n) = 2*m*n+m+n+2*V(m,n), for
 *          m >= 1, n >= 1.
 * https://oeis.org/A115010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) read by antidiagonals where T(m,n) = 2*m*n + m + n
 *            + 2*V(m,n) V(m,n) = Σ_{i=1..m} Σ_{j=1..n, gcd(i,j)=1}
 *            (m+1-i)*(n+1-j) The pair (m,n) for index n is obtained from the
 *            (k‑th) anti‑diagonal: k = smallest integer with k(k+1)/2 ≥ n
 *            offset = n - (k-1)k/2 - 1 i = offset + 1, j = k - offset
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef MAX_MU
#define MAX_MU 2000
#endif

/* Möbius function values from 0..MAX_MU */
static int mu[MAX_MU + 1] = {0};
static bool mu_computed = false;

/* Simple sieve to pre‑compute Möbius values up to MAX_MU */
static void init_mu(void)
{
    if (mu_computed) return;
    int limit = MAX_MU;
    for (int i = 0; i <= limit; ++i) mu[i] = 1;
    bool prime[limit + 1];
    for (int i = 0; i <= limit; ++i) prime[i] = true;
    prime[0] = prime[1] = false;
    for (int p = 2; p <= limit; ++p) {
        if (!prime[p]) continue;
        for (int j = p; j <= limit; j += p) {
            prime[j] = false;
            mu[j] = -mu[j];
        }
        for (long long sq = (long long)p * p; sq <= limit; sq += (long long)p * p)
            mu[sq] = 0;
    }
    mu_computed = true;
}

/* Compute V(m,n) using the derived formula with Möbius inversion */
static seq_t compute_V(idx_t m, idx_t n)
{
    idx_t minmn = (m < n) ? m : n;
    seq_t C = 0, Sj = 0, Si = 0, Sij = 0;
    for (idx_t d = 1; d <= minmn; ++d) {
        int mu_d = mu[d];
        if (mu_d == 0) continue;
        idx_t A = m / d;
        idx_t B = n / d;
        seq_t mu_s = (seq_t)mu_d;

        C += mu_s * (seq_t)A * (seq_t)B;

        seq_t sumB = (seq_t)B * (seq_t)(B + 1) / 2;
        Sj += mu_s * (seq_t)d * (seq_t)A * sumB;

        seq_t sumA = (seq_t)A * (seq_t)(A + 1) / 2;
        Si += mu_s * (seq_t)d * (seq_t)B * sumA;

        Sij += mu_s * (seq_t)d * (seq_t)d * sumA * sumB;
    }
    seq_t V = (seq_t)(m + 1) * (seq_t)(n + 1) * C
            - (seq_t)(m + 1) * Sj
            - (seq_t)(n + 1) * Si
            + Sij;
    return V;
}

/* Compute T(m,n) */
static seq_t compute_T(idx_t m, idx_t n)
{
    seq_t V = compute_V(m, n);
    seq_t T = 2 * (seq_t)m * (seq_t)n + (seq_t)m + (seq_t)n + 2 * V;
    return T;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, safe return */
    init_mu();

    /* Find the diagonal k such that k(k+1)/2 >= n */
    idx_t k = 1;
    idx_t total = 1;  /* k*(k+1)/2 for current k */
    while (total < n) {
        ++k;
        total = k * (k + 1) / 2;
    }

    /* Previous total up to diagonal k-1 */
    idx_t prev = (k - 1) * k / 2;
    idx_t offset = n - prev - 1;  /* zero based within diagonal */
    idx_t i = offset + 1;
    idx_t j = k - offset;

    return compute_T(i, j);
}
