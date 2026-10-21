/*
 * A076541  a(n) = Sum_{k=1..n} C(n,k) mod k.
 * https://oeis.org/A076541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(n,k) mod k
 * Range:     u8 n <= 38, u16 n <= 593, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_mod_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_mod_u32.h"

/* global prime table */
static idx_t *g_primes = NULL;
static size_t g_prime_count = 0;
static idx_t g_prime_limit = 0;

/* ensure primes up to limit (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_prime_limit)
        return;

    /* free old data */
    if (g_primes)
        free(g_primes);

    /* simple sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    bool *comp = (bool *)malloc(sz);
    if (!comp)
        return;          /* out of memory – silently fail */
    for (size_t i = 0; i < sz; ++i)
        comp[i] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!comp[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                comp[(size_t)j] = true;

    /* collect primes */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[(size_t)i])
            ++count;

    g_primes = (idx_t *)malloc(count * sizeof(idx_t));
    g_prime_count = count;
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[(size_t)i])
            g_primes[idx++] = i;

    g_prime_limit = limit;
    free(comp);
}

/* Legendre's formula: exponent of prime p in n! */
static inline uint32_t legendre(idx_t n, idx_t p)
{
    uint32_t e = 0;
    while (n)
    {
        n /= p;
        e += (uint32_t)n;
    }
    return e;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_primes(n);

    /* exponents of primes in n! */
    uint32_t *e_n = (uint32_t *)malloc(g_prime_count * sizeof(uint32_t));
    for (size_t pi = 0; pi < g_prime_count; ++pi)
        e_n[pi] = legendre(n, g_primes[pi]);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        uint32_t mod = (uint32_t)k;
        uint32_t res = 1 % mod;  /* C(n,k) mod k */

        for (size_t pi = 0; pi < g_prime_count; ++pi)
        {
            uint32_t exp =
                e_n[pi] -
                legendre(k, g_primes[pi]) -
                legendre(n - k, g_primes[pi]);

            if (exp == 0)
                continue;

            uint32_t term = pow_mod_u32((uint32_t)g_primes[pi], exp, mod);
            res = (uint32_t)((uint64_t)res * term % mod);
        }

        sum += (seq_t)res;
    }

    free(e_n);
    return sum;
}
