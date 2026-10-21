/*
 * A236288  a(n) = sigma(n)^n / sigma(n)^tau(n), where tau(n) = A000005(n) =
 *          the number of divisors of n and sigma(n) = A000203(n) = the sum of
 *          divisors of n.
 * https://oeis.org/A236288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n)^(n - tau(n)) where sigma(n) is the sum of
 *            divisors and tau(n) is the number of divisors
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 18, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

static uint64_t *g_primes = NULL;
static size_t g_prime_count = 0;

/* Simple sieve to fill g_primes once and reuse for all calls */
static void init_primes(void)
{
    if (g_primes != NULL)
        return;
    /* Enough primes for n up to at least 100000 (sqrt(1e10)=1e5) */
    build_primes_upto(100000, &g_primes, &g_prime_count);
}

/* Compute sigma(n) and tau(n) via prime factorisation */
static void sigma_tau(uint64_t n, uint64_t *sigma_out, uint32_t *tau_out)
{
    uint64_t sigma = 1;
    uint32_t tau = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < g_prime_count && (uint64_t)g_primes[i] * (uint64_t)g_primes[i] <= tmp; ++i)
    {
        uint64_t p = g_primes[i];
        if (tmp % p == 0)
        {
            unsigned int exp = 0;
            uint64_t term = 1;            /* term = 1 + p + p^2 + ... + p^exp */
            uint64_t p_pow = 1;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
                p_pow *= p;
                term += p_pow;
            }
            tau *= (exp + 1);
            sigma *= term;
        }
    }

    /* Remaining prime factor */
    if (tmp > 1)
    {
        tau *= 2;               /* exponent 1 -> (1+1) */
        sigma *= (1 + tmp);
    }

    *sigma_out = sigma;
    *tau_out = tau;
}

seq_t a(idx_t n)
{
    /* Lazily initialise primes */
    init_primes();

    /* Handle the trivial case n=1 separately to avoid any possible
       zero‑division or negative exponent issues */
    if (n == 1)
        return (seq_t)1;

    uint64_t sigma = 0;
    uint32_t tau = 0;
    sigma_tau((uint64_t)n, &sigma, &tau);

    seq_t exp = (seq_t)n - (seq_t)tau;
    if (exp == 0)
        return (seq_t)1;   /* sigma^0 = 1 */

    seq_t sigma_seq = (seq_t)sigma;
    return big_pow(sigma_seq, exp);
}
