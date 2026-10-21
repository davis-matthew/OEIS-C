/*
 * A051084  Primes p such that x^30 = -2 has a solution mod p.
 * https://oeis.org/A051084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64, mod_mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that x^30 = -2 has a solution modulo p.
   Implemented by sieving primes and testing the condition. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"
#include "../utils/mod_mul64.h"

/* 64‑bit modular exponentiation (binary powering) */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Euclidean gcd for 64‑bit integers */

/* Append a value to a dynamic uint64_t array */
static void push_uint64(uint64_t val, uint64_t **arr, size_t *len, size_t *cap)
{
    if (*len == *cap)
    {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Test whether a prime p satisfies (-2) has a 30th root modulo p */
static bool satisfies(uint64_t p)
{
    if (p == 2ULL)
        return true;                 /* 0^30 ≡ -2 (mod 2) */
    uint64_t base = p - 2ULL;        /* (-2) mod p */
    uint64_t d    = gcd64(30ULL, p - 1ULL);
    uint64_t exp  = (p - 1ULL) / d;
    return mod_pow64(base, exp, p) == 1ULL;
}

/* External linkage */
seq_t a(idx_t n)
{
    /* Persistent state across calls */
    static uint64_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static uint64_t *good   = NULL;
    static size_t good_len = 0, good_cap = 0;
    static uint64_t candidate = 3ULL;   /* next odd number to test */

    /* Initialise on first call */
    if (primes == NULL)
    {
        /* Prime 2 */
        push_uint64(2ULL, &primes, &primes_len, &primes_cap);
        if (satisfies(2ULL))
            push_uint64(2ULL, &good, &good_len, &good_cap);
    }

    /* Generate primes until we have at least n good primes */
    while (good_len < (size_t)n)
    {
        uint64_t c = candidate;
        bool is_prime = true;

        /* Trial division by stored primes up to sqrt(c) */
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint64_t p = primes[i];
            if (p > c / p)          /* avoid overflow, p*p > c */
                break;
            if (c % p == 0ULL)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            push_uint64(c, &primes, &primes_len, &primes_cap);
            if (satisfies(c))
                push_uint64(c, &good, &good_len, &good_cap);
        }

        candidate += 2ULL;   /* skip even numbers */
    }

    return (seq_t)good[n - 1ULL];   /* 0‑based index into the good array */
}
