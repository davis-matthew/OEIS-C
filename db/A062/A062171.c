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
 * Helpers:   count_distinct_primes, divisor_count_u64
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

#include "../utils/divisor_count_u64.h"
#include "../utils/count_distinct_primes.h"

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
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
