/*
 * A175622  a(n) = 2^(semiprime(n)-4) mod semiprime(n).
 * https://oeis.org/A175622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2^(semiprime(n)-4) mod semiprime(n)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th semiprime by simple trial division and then uses
   binary exponentiation to get 2^(p-4) mod p, where p is that semiprime. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* 64‑bit modular exponentiation: result = base^exp mod mod. */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* Static cache for the first N semiprimes (uint64_t values). */
static uint64_t *semiprimes = NULL;   /* array of semiprime values   */
static size_t sem_cap = 0;            /* current capacity            */
static size_t sem_cnt = 0;            /* number of stored semiprimes */

/* Prime list for factoring numbers up to a bound. */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Next candidate number to test for being a semiprime. */
static uint64_t next_candidate = 4;   /* 2*2 is the first semiprime */

/* Initialise the prime table if it hasn't been done yet. */
static void init_primes(void)
{
    if (prime_cnt != 0)
        return;
    /* Enough primes to factor numbers up to roughly a few hundred thousand. */
    build_primes_upto(300000, &primes, &prime_cnt);
}

/* Test whether x is a semiprime (product of exactly two primes, counted with multiplicity). */
static int is_semiprime(uint64_t x)
{
    if (x < 4)
        return 0;
    int factor_cnt = 0;
    uint64_t remaining = x;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > remaining)
            break;
        while (remaining % p == 0)
        {
            ++factor_cnt;
            remaining /= p;
            if (factor_cnt > 2)
                return 0;
        }
    }
    if (remaining > 1)
        ++factor_cnt; /* remaining prime factor > sqrt(x) */
    return factor_cnt == 2;
}

/* Ensure that the cache contains at least n semiprimes. */
static void ensure_semiprimes(idx_t n)
{
    if (sem_cnt >= (size_t)n)
        return;
    init_primes();

    if (sem_cap < (size_t)n)
    {
        size_t new_cap = sem_cap ? sem_cap * 2 : 64;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(semiprimes, new_cap * sizeof(uint64_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        semiprimes = tmp;
        sem_cap = new_cap;
    }

    while (sem_cnt < (size_t)n)
    {
        while (!is_semiprime(next_candidate))
            ++next_candidate;
        semiprimes[sem_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* out of range guard */

    ensure_semiprimes(n);
    uint64_t sem = semiprimes[n - 1];

    /* Compute 2^(sem-4) mod sem. sem is at least 4, so exponent is non‑negative. */
    uint64_t exponent = (sem > 4) ? sem - 4 : 0;
    uint64_t result = modpow_uint64(2, exponent, sem);

    return (seq_t)result;
}
