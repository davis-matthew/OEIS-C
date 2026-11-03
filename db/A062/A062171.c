/*
 * A062171  Numbers k such that number of non-unitary divisors of k (A048105)
 *          > number of distinct prime divisors of k (A001221).
 * https://oeis.org/A062171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9683 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062171 –  Numbers k such that
   d(k) > 2^ω(k) + ω(k)
   (d(k) = divisor_count_u64(k), ω(k) = count_distinct_primes(k)).
   The function builds the sequence on demand, memoising the terms
   in a cache that can be accessed non‑linearly. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count divisors of a 64‑bit integer */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t exp;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++exp;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    return tau;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* The build supplies seq_t and idx_t; do NOT redefine them. */

/* Cache for the sequence values */
static seq_t *cache      = NULL; /* array of computed terms          */
static size_t cache_cap  = 0;    /* current capacity of cache        */
static size_t cache_len  = 0;    /* number of terms already stored   */

/* Next integer that will be tested for the property */
static uint64_t next_candidate = 1;

/* Ensure cache has room for at least one more element */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether a given integer satisfies the property */
static bool satisfies(uint64_t n)
{
    if (n <= 1) return false;          /* 0 and 1 are never needed */

    /* d(n)  – number of divisors (including 1 and n)          */
    /* ω(n) – number of distinct primes dividing n             */
    seq_t d = divisor_count_u64(n);
    int   omega = count_distinct_primes(n);

    /* Compute 2^ω(n) as a seq_t power of two.                     */
    seq_t two_pow = (seq_t)1 << omega;

    /* Property: d(n) > 2^ω(n) + ω(n)                            */
    return d > two_pow + (seq_t)omega;
}

/* Main sequence function required by the task */
seq_t a(idx_t n)
{
    /* Validate that n is in the legal range for the offset */
    if (n == 0) abort();   /* offset 1 – a(1) is first term */

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n-1];

    /* Extend cache until we have n terms */
    while (cache_len < n) {
        /* Find next integer that satisfies the property */
        do { ++next_candidate; } while (!satisfies(next_candidate));

        /* Store it in the cache */
        ensure_cache_capacity();
        cache[cache_len++] = (seq_t)next_candidate;
    }

    return cache[n-1];
}
