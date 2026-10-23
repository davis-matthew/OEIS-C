/*
 * A163778  Odd terms in A054639.
 * https://oeis.org/A163778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 4599, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n‑th odd integer N such that p = 2N+1 is prime and
 * exactly one of +2 or –2 generates the multiplicative group Z_p^*.
 * Implementation uses a prime sieve up to 10 000 000 and tests
 * primitive‑root conditions by factorisation of p–1. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Helper function: modular exponentiation (g^e mod m) */
static uint64_t mod_pow(uint64_t g, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    g %= m;
    while (e)
    {
        if (e & 1)
            r = (unsigned __int128)r * g % m;
        g = (unsigned __int128)g * g % m;
        e >>= 1;
    }
    return r;
}

/* Test whether g is a primitive root modulo prime p.
 * primes and primes_count provide a precomputed list of primes
 * up to at least sqrt(p).  The factorisation of p-1 is obtained
 * by trial division using that list. */
static bool is_primitive_root(uint64_t g, uint64_t p,
                              const uint32_t *primes, size_t primes_count)
{
    uint64_t phi = p - 1;      /* p is prime, so p-1 = phi(p) */
    uint64_t m   = phi;

    for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * primes[i] <= m; ++i)
    {
        uint32_t q = primes[i];
        if (m % q == 0)
        {
            if (mod_pow(g, phi / q, p) == 1)
                return false;
            while (m % q == 0)
                m /= q;
        }
    }
    if (m > 1)  /* remaining prime factor */
    {
        if (mod_pow(g, phi / m, p) == 1)
            return false;
    }
    return true;
}

/* Prime sieve up to LIMIT.  Allocated the first time a() is called. */
#define SIEVE_LIMIT 10000000ULL
static bool   *sieve_arr  = NULL;
static uint32_t *prime_arr = NULL;
static size_t    prime_cnt = 0;
static bool       sieve_inited = false;

static void init_sieve(void)
{
    size_t n = (size_t)SIEVE_LIMIT;
    sieve_arr = (bool *)malloc(n + 1);
    if (!sieve_arr) exit(1);
    memset(sieve_arr, 1, n + 1);
    sieve_arr[0] = sieve_arr[1] = 0;

    uint32_t sq = (uint32_t)sqrt((double)n);
    for (uint32_t i = 2; i <= sq; ++i)
        if (sieve_arr[i])
            for (uint64_t j = (uint64_t)i * i; j <= n; j += i)
                sieve_arr[j] = 0;

    /* collect primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= n; ++i)
        if (sieve_arr[i])
            ++cnt;
    prime_arr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_arr) exit(1);
    size_t idx = 0;
    for (uint32_t i = 2; i <= n; ++i)
        if (sieve_arr[i])
            prime_arr[idx++] = i;
    prime_cnt = cnt;
    sieve_inited = true;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (!sieve_inited)
        init_sieve();

    size_t found = 0;
    uint64_t N = 1;  /* start with N = 1 (odd) */
    for (;; N += 2)
    {
        uint64_t p = 2 * N + 1;
        if (p <= SIEVE_LIMIT && !sieve_arr[p])
            continue;          /* p is not prime */

        /* For p > SIEVE_LIMIT, a quick Miller–Rabin test could be added,
         * but for the intended n <= 10000 the limits above are sufficient. */

        bool root2  = is_primitive_root(2,          p, prime_arr, prime_cnt);
        bool rootm2 = is_primitive_root(p - 2,      p, prime_arr, prime_cnt);
        if (root2 ^ rootm2)
        {
            ++found;
            if (found == n)
                return (seq_t)N;
        }
    }
}
