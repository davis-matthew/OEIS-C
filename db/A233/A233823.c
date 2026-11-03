/*
 * A233823  Least prime in arithmetic progression of consecutive primes 6^n+k,
 *          6^n+k+g, 6^n+k+2*g with smallest k.
 * https://oeis.org/A233823
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds a(n) = 6^n + the smallest k>0 such that
   6^n + k, 6^n + k + g, 6^n + k + 2g are consecutive primes
   for some integer g.  The function searches for the first
   prime p>6^n that starts a 3‑term arithmetic progression of
   consecutive primes.  It returns that prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* Multiply-by-self exponentiation for big exponent (seq_t type) */
static seq_t big_pow(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp != 0) {
        if ((exp % 2) != 0) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; for n < 2 behaviour
       is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* Compute 6^n.  big_pow accepts seq_t arguments. */
    const seq_t six   = (seq_t)6;
    seq_t      base   = big_pow(six, (seq_t)n);

    /* The first candidate prime is the smallest odd number
       greater than base.  base is even for n>=1. */
    seq_t candidate = base + (seq_t)1;   /* odd */

    /* Fast loop: skip even numbers by adding 2 each step. */
    while (true)
    {
        /* Find next prime >= candidate. */
        while (!isPrimeGeneric(candidate))
            candidate += (seq_t)2;       /* stay odd */

        /* candidate is a prime p1.  Find the next two consecutive
           primes p2 and p3. */
        seq_t p1 = candidate;

        /* Find p2. */
        seq_t p2 = p1 + (seq_t)1;
        if ((p2 & (seq_t)1) == (seq_t)0)   /* make odd */
            p2 += (seq_t)1;
        while (!isPrimeGeneric(p2))
            p2 += (seq_t)2;

        /* Find p3. */
        seq_t p3 = p2 + (seq_t)1;
        if ((p3 & (seq_t)1) == (seq_t)0)
            p3 += (seq_t)1;
        while (!isPrimeGeneric(p3))
            p3 += (seq_t)2;

        /* Check if the three primes form an arithmetic progression. */
        seq_t g1 = p2 - p1;
        seq_t g2 = p3 - p2;
        if (g1 == g2)
            return p1;   /* this is a(n) */

        /* No progression found starting with p1; continue searching
           from the next odd number after p1. */
        candidate = p1 + (seq_t)2;
    }
}
