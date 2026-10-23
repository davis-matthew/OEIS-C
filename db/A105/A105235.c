/*
 * A105235  Partial sums of the central column of a Moebius-binomial triangle.
 * https://oeis.org/A105235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n – (number of even squarefree integers ≤ n)
 * Range:     u8 n <= 319, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes the OEIS sequence A105235.
   It keeps a static Möbius array and recomputes it with a linear
   sieve whenever larger values are requested.  The direct
   summation of even squarefree numbers yields sufficient
   speed for n up to at least 10⁴. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static Möbius table:  μ[0] unused, μ[1] = 1 */
static int8_t *mu = NULL;
static size_t mu_size = 0;

/* Recompute μ[0..N] with a linear sieve whenever N grows. */
static void compute_mu_upto(size_t N)
{
    if (N <= mu_size) return;
    int8_t *new_mu = (int8_t *)realloc(mu, (N + 1) * sizeof(int8_t));
    if (!new_mu) abort();
    mu = new_mu;

    mu[0] = 0;
    if (N >= 1) mu[1] = 1;

    /* Linear sieve auxiliary arrays */
    char *is_comp = (char *)calloc(N + 1, sizeof(char));
    if (!is_comp) abort();
    int *primes = NULL;
    size_t primes_cap = 0, primes_cnt = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                int *tmp = (int *)realloc(primes, primes_cap * sizeof(int));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = (size_t)primes[j];
            size_t x = i * p;
            if (x > N) break;
            is_comp[x] = 1;
            if (i % p == 0) {
                mu[x] = 0;
                break;
            } else {
                mu[x] = -mu[i];
            }
        }
    }
    free(primes);
    free(is_comp);
    mu_size = N;
}

/* Exported sequence function */
seq_t a(idx_t n)
{
    /* Ensure the Möbius table covers 0..n */
    compute_mu_upto((size_t)n);

    seq_t count = 0;            /* count of even squarefree integers ≤ n */
    for (idx_t k = 2; k <= n; k += 2) {
        if (mu[k] != 0) ++count;
    }

    seq_t res = (seq_t)(n + 1);
    res -= count;
    return res;
}
