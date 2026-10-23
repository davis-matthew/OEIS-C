/*
 * A019408  Primes with primitive root 86.
 * https://oeis.org/A019408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that has 86 as a primitive root.
   The sequence starts 3, 19, 23, 31, … (OEIS A019408). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- helper functions ---------- */

/* fast modular exponentiation: returns (base^exp) mod mod.
   Assumes mod < 2^64, so product fits into unsigned __int128. */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Simple primality test by trial division.  Works for all primes needed
   (the 10 000th prime is 104729, so the loop is very fast). */
static bool is_prime(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2ULL == 0)  return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL)
            return false;
    return true;
}

/* Test whether 86 is a primitive root modulo the prime p (p > 2). */
static bool is_primitive_root_86(uint64_t p)
{
    if (p == 2ULL || p == 43ULL)   /* 86 shares a factor with p */
        return false;

    uint64_t g = 86ULL % p;        /* primitive root candidate */
    uint64_t m = p - 1ULL;
    /* small list of distinct prime divisors of m (m < 2^64, but m < 2^20 for needed primes) */
    uint64_t q;
    /* trial division to find distinct prime factors */
    for (q = 2ULL; q * q <= m; q = (q == 2ULL) ? 3ULL : q + 2ULL) {
        if (m % q == 0ULL) {
            while (m % q == 0ULL) m /= q;    /* remove all powers */
            if (modpow64(g, (p - 1ULL) / q, p) == 1ULL)
                return false;
        }
    }
    if (m > 1ULL) {          /* remaining prime divisor */
        if (modpow64(g, (p - 1ULL) / m, p) == 1ULL)
            return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* index 0 is unused per OEIS offset 1 */
    if (n == 0) return (seq_t)0;

    /* static cache for found primes */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t last_checked = 1ULL;   /* last number examined */

    /* grow cache until we have at least n entries */
    while (len < (size_t)n) {
        /* find next candidate prime */
        do {
            last_checked += 1ULL;
        } while (!is_prime(last_checked));

        if (is_primitive_root_86(last_checked)) {
            /* add to cache */
            if (len + 1 > cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* fatal error */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)last_checked;
        }
    }

    return cache[len - 1];
}
