/*
 * A106846  a(n) = Sum_{k + l*m <= n} (k + l*m), with 0 <= k,l,m <= n.
 * https://oeis.org/A106846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * ( n*(n+1)*(2*n+1) * + Σ_{k=1..n} ( n^2 + n + k -
 *            k^2 ) * τ(k) )
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(k) is the number of positive divisors of k. The sequence is
 * computed from the above formula using a simple divisor‑count sieve. */

#include <stdlib.h>
#include <stdint.h>

/* ---------- divisor‑count cache ---------- */
static int *tau_cache = NULL;       /* tau_cache[i] = τ(i) for 0 <= i <= cached_max */
static idx_t tau_cache_max = 0;     /* maximum i cached */

static void extend_tau(idx_t n)
{
    if (n <= tau_cache_max) return;
    /* allocate a new array big enough for 0..n (inclusive) */
    int *new_tau = (int *)calloc((size_t)(n + 1), sizeof(int));
    if (!new_tau) return;           /* allocation failed – leave cache empty */

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++new_tau[(size_t)j];
        }
    }
    free(tau_cache);
    tau_cache = new_tau;
    tau_cache_max = n;
}

/* ---------- sequence itself ---------- */
seq_t a(idx_t n)
{
    /* 0 ≤ n.  For n = 0 the result is 0. */
    if (n == 0) return (seq_t)0;

    /* precompute τ(k) for 1 ≤ k ≤ n */
    extend_tau(n);

    /* convert n to seq_t once */
    seq_t n_t = (seq_t)n;

    /* term1 = n*(n+1)*(2*n+1)  (all in seq_t) */
    seq_t term1 = n_t * (n_t + (seq_t)1) * (((seq_t)2 * n_t) + (seq_t)1);

    /* sum = Σ_{k=1..n} ( n^2 + n + k - k^2 ) * τ(k) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_t = (seq_t)k;
        seq_t part = (n_t * n_t) + n_t + k_t - (k_t * k_t);
        sum += part * (seq_t)tau_cache[(size_t)k];
    }

    seq_t total = term1 + sum;
    /* division by 2 is exact */
    return total / (seq_t)2;
}
