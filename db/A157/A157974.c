/*
 * A157974  Primes p such that 12*p + 11 is also prime.
 * https://oeis.org/A157974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1527, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that 12*p + 11 is also prime */
/* Offset: 1 (a(1) = 3, a(2) = 5, …) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers.
   Adapted from the known bases that are sufficient for all n < 2^64. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    /* Small prime divisors */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)   return n == p;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Witnesses that are enough for n < 2^64 */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) return true;

        /* Compute a^d mod n with 128-bit multiplication */
        __uint128_t x = 1;
        __uint128_t base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool passed = false;
        for (unsigned r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                passed = true;
                break;
            }
        }
        if (!passed) return false;
    }
    return true;
}

/* Storage for the qualifying primes */
static seq_t *qual_list   = NULL;
static size_t qual_cnt    = 0;
static size_t qual_cap    = 0;

/* Next odd number to test as a candidate prime */
static uint64_t next_candidate = 3;

/* Initialise static data on first use */
static void init_static(void)
{
    if (qual_list != NULL) return;   /* already initialised */

    qual_cap = 16;
    qual_list = (seq_t *)malloc(qual_cap * sizeof(seq_t));
    qual_cnt = 0;
}

/* Append a new qualifying prime to qual_list, expanding if necessary */
static void append_qual(uint64_t p)
{
    if (qual_cnt == qual_cap) {
        qual_cap *= 2;
        qual_list = (seq_t *)realloc(qual_list,
                                     qual_cap * sizeof(seq_t));
    }
    qual_list[qual_cnt++] = (seq_t)p;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0, return 0 */

    init_static();

    /* Find at least n qualifying primes */
    while (qual_cnt < (size_t)n) {
        uint64_t p = next_candidate;

        /* Find the next prime p */
        while (!is_prime_uint64(p))
            p += 2;                /* skip even numbers */

        /* Check 12p + 11 */
        __uint128_t q128 = (__uint128_t)12 * p + 11;
        if (q128 <= ( __uint128_t)UINT64_MAX) {
            uint64_t q = (uint64_t)q128;
            if (is_prime_uint64(q))
                append_qual(p);
        }

        /* Prepare for the next candidate */
        next_candidate = p + 2;
    }

    return qual_list[(size_t)n - 1];
}
