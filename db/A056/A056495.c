/*
 * A056495  Number of primitive (period n) periodic palindromes using a
 *          maximum of four different symbols.
 * https://oeis.org/A056495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * A056486(n/d) A056486(k) = 4^((k+1)/2) if k
 *            odd 4^(k/2) + 6·4^(k/2-1) if k even (derived from the OEIS
 *            entry)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

/* --- cache for Möbius function μ up to the maximum n needed --- */
static int    *mu_cache     = NULL;   /* μ[0..max]  */
static idx_t  mu_max       = 0;

/* ensure μ up to n is computed */
static void
ensure_mu(idx_t n)
{
    if (n <= mu_max) return;

    int *mu = (int *)calloc((size_t)(n + 1), sizeof(int));
    if (!mu) abort();

    /* basic linear sieve for μ */
    int  *prime = (int *)malloc((size_t)(n + 1) * sizeof(int));
    size_t prime_cnt = 0;
    char *is_composite = (char *)calloc((size_t)(n + 1), sizeof(char));
    if (!prime || !is_composite) abort();

    mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_composite[i]) {
            prime[prime_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = (idx_t)prime[j];
            idx_t ip = i * p;
            if (ip > n) break;
            is_composite[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    free(prime);
    free(is_composite);

    free(mu_cache);
    mu_cache = mu;
    mu_max   = n;
}

/* --- cache for powers of 4 --- */
static seq_t *pow4_cache    = NULL;   /* 4^i for i=0..max */
static idx_t  pow4_max     = 0;

/* ensure powers of 4 up to max are computed */
static void
ensure_pow4(idx_t max)
{
    if (max <= pow4_max) return;

    seq_t *new_pow4 = (seq_t *)malloc((size_t)(max + 1) * sizeof(seq_t));
    if (!new_pow4) abort();

    new_pow4[0] = (seq_t)1;
    for (idx_t i = 1; i <= max; ++i)
        new_pow4[i] = mul_small(new_pow4[i - 1], 4);

    free(pow4_cache);
    pow4_cache = new_pow4;
    pow4_max   = max;
}

/* --- helper: compute A056486(k) using the closed formula --- */
static inline seq_t
A056486(idx_t k)
{
    if (k & 1U) {                      /* k is odd  */
        idx_t p = (k + 1U) >> 1;       /* (k+1)/2 as idx_t   */
        return pow4_cache[p];
    } else {                            /* k is even */
        idx_t t = k >> 1;              /* t = k/2            */
        seq_t res = pow4_cache[t];
        seq_t term6 = mul_small(pow4_cache[t - 1], 6);
        res += term6;
        return res;
    }
}

/* --------- the required sequence function --------- */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* outside offset – undefined */

    ensure_mu(n);
    ensure_pow4((n >> 1) + 1);          /* up to ⌊n/2⌋+1   */

    seq_t pos = (seq_t)0;               /* accumulate positive contributions */
    seq_t neg = (seq_t)0;               /* accumulate negative contributions */

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        idx_t d1 = i;
        idx_t d2 = n / i;

        /* contribution from divisor d1 */
        seq_t val1 = A056486(n / d1);
        if (mu_cache[d1] == 1) pos += val1;
        else if (mu_cache[d1] == -1) neg += val1;

        /* contribution from divisor d2 (if distinct) */
        if (d2 != d1) {
            seq_t val2 = A056486(n / d2);
            if (mu_cache[d2] == 1) pos += val2;
            else if (mu_cache[d2] == -1) neg += val2;
        }
    }

    return pos - neg;
}
