/*
 * A049567  Primes p such that x^35 = 2 has a solution mod p.
 * https://oeis.org/A049567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that the congruence x^35 ≡ 2 (mod p) has a
 *            solution
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the nth such prime (offset 1). */
/* It uses deterministic Miller‑Rabin for 64‑bit integers and 128‑bit arithmetic
   for modular multiplication. No external helpers are required. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached qualifying primes */
static seq_t *qual_primes = NULL;       /* array of cached primes */
static size_t qual_len = 0;            /* number of cached primes */
static size_t qual_cap = 0;            /* capacity of the cache */
static uint64_t next_prime_to_test = 0;/* next candidate prime to examine */

/* 128‑bit modular multiplication: (a*b) % mod */
static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* 64‑bit modular exponentiation: base^exp mod mod */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp != 0)
    {
        if (exp & 1ULL)
            result = mod_mul64(result, b, mod);
        b = mod_mul64(b, b, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Euclid algorithm for 64‑bit integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0ULL)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Deterministic Miller‑Rabin primality test for 64‑bit integers */
static bool is_prime64(uint64_t n)
{
    if (n < 2ULL)
        return false;
    if (n % 2ULL == 0ULL)
        return n == 2ULL;
    if (n % 3ULL == 0ULL)
        return n == 3ULL;

    /* Write n-1 as d*2^s with d odd */
    uint64_t d = n - 1ULL;
    int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        s++;
    }

    /* Bases that are sufficient for 64‑bit integers */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL,
                              11ULL, 13ULL, 17ULL, 19ULL,
                              23ULL, 29ULL, 31ULL, 37ULL};
    const size_t nbases = sizeof(bases) / sizeof(bases[0]);

    for (size_t i = 0; i < nbases; ++i)
    {
        uint64_t a = bases[i];
        if (a >= n)
            continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1ULL || x == n - 1ULL)
            continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul64(x, x, n);
            if (x == n - 1ULL)
            {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, 0 is unused */

    /* Ensure cache contains at least n qualifying primes */
    while (qual_len < (size_t)n)
    {
        /* Find next prime candidate */
        if (next_prime_to_test == 0ULL)
            next_prime_to_test = 2ULL;
        else
            next_prime_to_test = (next_prime_to_test == 2ULL) ? 3ULL
                                                             : next_prime_to_test + 2ULL;

        uint64_t p = next_prime_to_test;
        if (!is_prime64(p))
            continue;   /* skip composites */

        /* Check the congruence condition */
        bool ok;
        if (p == 2ULL)
            ok = true;                    /* 2^35 ≡ 0 ≡ 2 (mod 2) */
        else
        {
            uint64_t g = gcd_uint64(35ULL, p - 1ULL);
            uint64_t exp = (p - 1ULL) / g;
            ok = (mod_pow64(2ULL, exp, p) == 1ULL);
        }

        if (ok)
        {
            /* Expand cache if necessary */
            if (qual_len == qual_cap)
            {
                qual_cap = (qual_cap == 0) ? 8 : qual_cap * 2;
                qual_primes = (seq_t *)realloc(qual_primes,
                                               qual_cap * sizeof(seq_t));
            }
            qual_primes[qual_len++] = (seq_t)p;
        }
    }

    return qual_primes[n - 1];
}
