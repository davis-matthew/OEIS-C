/*
 * A096787  Primes of form 4n+3 that are the sum of two consecutive composite
 *          numbers.
 * https://oeis.org/A096787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime of the form 4k+3 that equals m + (m+1)
 *            where m and m+1 are both composite
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
 
/* 128‑bit modular multiplication for use in Miller‑Rabin */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}
 
/* 128‑bit modular exponentiation */
static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e)
    {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}
 
/* Deterministic Miller‑Rabin test for 64‑bit integers
   (bases 2,325,9375,28178,450775,9780504,1795265022). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}
 
/* Composite test for 64‑bit integers */
static bool is_composite_uint64(uint64_t n)
{
    return n > 1 && !is_prime_uint64(n);
}
 
seq_t a(idx_t n)
{
    /* Undefined for indices < 1; return 0 in that case. */
    if (n < 1) return (seq_t)0;
 
    idx_t count = 0;
    uint64_t m = 4; /* first candidate m > 3 */
 
    /* Search until we find the n-th term. */
    while (count < n)
    {
        if (is_composite_uint64(m) && is_composite_uint64(m + 1))
        {
            uint64_t p = m + (m + 1); /* 2*m + 1 */
            if ((p & 3ULL) == 3 && is_prime_uint64(p))
            {
                ++count;
                if (count == n) return (seq_t)p;
            }
        }
        ++m;
    }
    /* Should never reach here. */
    return (seq_t)0;
}
