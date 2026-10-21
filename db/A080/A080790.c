/*
 * A080790  Binary emirps, primes whose binary reversal is a different prime.
 * https://oeis.org/A080790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev_bin_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binary Emirps: a(n) is the nth prime p such that the binary reversal of p is
 * a different prime. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rev_bin_uint64.h"

/* 128‑bit modular multiplication (GCC/Clang extension, also available in
 * C++17).  It works for 64‑bit bases on any platform that supports __uint128_t. */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Fast modular exponentiation. */
static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64‑bit integers.
 * Uses the well‑known set of bases that is correct for all 64‑bit numbers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Trial division by small primes first. */
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
         23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Write n-1 as d*2^s with d odd. */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases (see https://miller-rabin.appspot.com/). */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL,
         1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1)
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

/* Main sequence function.  No static storage is required; the function
 * simply searches from 3 upwards until the n‑th binary emirp is found. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, return 0 as fallback */

    idx_t count = 0;
    uint64_t candidate = 3;       /* primes > 2 are odd */

    while (1)
    {
        if (is_prime_uint64(candidate))
        {
            uint64_t rev = rev_bin_uint64(candidate);
            if (rev != candidate && is_prime_uint64(rev))
            {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        candidate += 2;           /* skip even numbers */
    }
}
