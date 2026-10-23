/*
 * A160129  a(n) = least prime p such that both p-+6*n are prime.
 * https://oeis.org/A160129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that both p-6*n and p+6*n are prime
 * Range:     u8 n <= 20, u16 n <= 6551, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Utility function to generate all primes up to a bound. */
#include "../utils/sieve_primes.h"
/* Utility function to test prime of 64‑bit integers. */
#include "../utils/is_prime_small.h"

static idx_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;     /* number of primes in the array */
static bool primes_initialized = false;
static const uint64_t PRIME_LIMIT = 20000000ULL;  /* 20 million should cover n ≤ 10000 */

/* Return the first index in prime_list with value >= val. */
static size_t first_prime_ge(uint64_t val)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((uint64_t)prime_list[mid] >= val) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the prime list is built once. */
    if (!primes_initialized) {
        size_t cnt = 0;
        sieve_primes(PRIME_LIMIT, &prime_list, &cnt);
        prime_count = cnt;
        primes_initialized = true;
    }

    /* n should be ≥ 1; for safety, clamp to 1. */
    if (n < 1) n = 1;

    /* Starting point for candidate primes. */
    uint64_t offset  = 6ULL * (uint64_t)n;
    uint64_t start   = offset + 2ULL;   /* p must be > 6*n, and at least 2. */
    if (start < 2ULL) start = 2ULL;

    /* Search over precomputed primes. */
    size_t idx = first_prime_ge(start);
    for (; idx < prime_count; ++idx) {
        uint64_t p   = (uint64_t)prime_list[idx];
        uint64_t low = p - offset;
        uint64_t high = p + offset;

        /* All numbers involved are ≤ PRIME_LIMIT for n ≤ 10000. */
        if (is_prime_small(p) && is_prime_small(low) && is_prime_small(high)) {
            return (seq_t)p;
        }
    }

    /* Fallback: if not found within PRIME_LIMIT (unlikely), do a brute force search. */
    /* This code path should never be executed for the expected input range. */
    for (uint64_t p = start; ; p += 2) {
        if (is_prime_small(p) &&
            is_prime_small(p - offset) &&
            is_prime_small(p + offset)) {
            return (seq_t)p;
        }
    }
}
