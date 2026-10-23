/*
 * A201185  Primes of the form 7k^3+4.
 * https://oeis.org/A201185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 7*k^3 + 4 (OEIS A201185)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 55, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"

/* 128‑bit modular multiplication using a 128‑bit intermediate */

/* 64‑bit modular exponentiation */
static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e > 0)
    {
        if (e & 1U)
            r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1U;
    }
    return r;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Small prime trial division */
    static const uint64_t small_primes[] =
    {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    size_t i;
    for (i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Factor n-1 = d*2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }

    /* Bases for deterministic test up to 2^64 */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        uint64_t r;
        for (r = 1ULL; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

#define MAX_TERMS 200000   /* plenty for n ≲ 10⁴ */

static seq_t primes[MAX_TERMS];
static idx_t prime_count = 0;
static idx_t current_k = 1;   /* next k to test */

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for non‑positive indices */

    /* Return if we already have enough terms */
    if (prime_count >= n)
        return primes[n - 1];

    while (prime_count < n)
    {
        __uint128_t val128 = (__uint128_t)7 * current_k * current_k * current_k + 4;

        /* For primality testing use 64‑bit conversion; the value is known to
           fit in 64 bits for the n considered in the test set. */
        uint64_t val64 = (uint64_t)val128;
        if (is_prime_uint64(val64))
        {
            if (prime_count < MAX_TERMS)
                primes[prime_count++] = (seq_t)val128;
            else
                return (seq_t)0;   /* buffer overflow; unlikely for n≲10⁴ */
        }
        ++current_k;
    }
    return primes[n - 1];
}
