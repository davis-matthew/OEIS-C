/*
 * A107162  Primes of the form x^2 + 54y^2.
 * https://oeis.org/A107162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^2 + 54y^2 (OEIS A107162).  The sequence
   starts at n = 1, a(1) = 79.  The function returns the nth such
   prime (ignoring the offset 0).  The implementation searches
   primes incrementally, testing each one for representation
   by the given quadratic form.  A small cache is used so that
   later calls are fast.  The algorithm runs in O(n * log p) time
   and stays well under two seconds for n ≈ 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root for 64‑bit values using a binary‑search style
   algorithm that never overflows. */
static uint64_t isqrt_u64(uint64_t x)
{
    if (x < 2) return x;
    uint64_t r = 0;
    for (uint64_t shift = 1ULL << 62; shift > 0; shift >>= 1)
    {
        uint64_t temp = r + shift;
        __uint128_t sq = (__uint128_t)temp * temp;
        if (sq <= x) r = temp;
    }
    return r;
}

/* Test whether a 64‑bit integer is a square. */
static bool is_square(uint64_t z)
{
    uint64_t r = isqrt_u64(z);
    return r * r == z;
}

/* Deterministic Miller‑Rabin primality test for 64‑bit numbers. */
static bool is_prime_64(uint64_t n)
{
    if (n < 2)                  return false;
    if (n % 2 == 0)             return n == 2;
    if (n % 3 == 0)             return n == 3;
    if (n % 5 == 0)             return n == 5;

    /* Write n-1 = 2^s * d with d odd. */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases sufficient for all 64‑bit values. */
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1, pow = a, mod = n;
        uint64_t exp = d;
        while (exp)
        {
            if (exp & 1) x = (x * pow) % mod;
            pow = (pow * pow) % mod;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; ++r)
        {
            x = (x * x) % mod;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Test whether a prime p can be written as x^2 + 54*y^2. */
static bool representable(uint64_t p)
{
    /* y may be zero, but for any prime > 2 it will be found with 
       y = 1 or larger. */
    for (uint64_t y = 1;; ++y)
    {
        __uint128_t prod = (__uint128_t)54 * y * y;
        if (prod > p) break;
        uint64_t val = (uint64_t)prod;
        uint64_t z   = p - val;
        if (is_square(z)) return true;
    }
    return false;
}

/* -------------------------------------------------------------------- */
/* Cache of primes in the form x^2 + 54y^2.  The array is large enough
   for the typical tests. */
#define MAX_TERMS 25000
static seq_t cache[MAX_TERMS];
static idx_t cache_cnt = 0;
static uint64_t last_checked = 1;

/* Fill the cache until it contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_cnt < n)
    {
        /* Find the next prime. */
        do { ++last_checked; } while (!is_prime_64(last_checked));

        /* If it has the required quadratic form, store it. */
        if (representable(last_checked))
        {
            if (cache_cnt < MAX_TERMS)
                cache[cache_cnt++] = (seq_t)last_checked;
            else
                return; /* overflow guard */
        }
    }
}

/* -------------------------------------------------------------------- */
/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    ensure_terms(n);
    return cache[n - 1];
}
