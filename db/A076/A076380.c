/*
 * A076380  Sum of digits in base 2 is a divisor of sum of prime divisors
 *          (A008472).
 * https://oeis.org/A076380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer x such that popcount(x) divides the sum
 *            of distinct prime divisors of x
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* popcount for 64‑bit unsigned integer (works on any width) */
static inline uint32_t popcount_uint64(uint64_t x)
{
    uint32_t c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
}

/* array of smallest prime factors (unsigned 64‑bit) */
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
static size_t spf_cap = 0;         /* number of entries currently stored */

/* Ensure that spf contains factors up to n (inclusive).  The old
   array is freed and replaced if needed. */
static void update_spf(idx_t n)
{
    if ((idx_t)n < spf_cap) return;
    if (spf) free(spf);
    build_spf(n, &spf);               /* allocates new array up to n */
    spf_cap = (size_t)n + 1;
}

/* Cache of discovered terms of the sequence */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of stored terms */
static size_t cache_cap = 0;         /* capacity of cache array */

/* Grow the cache array when required.  The cache holds the terms
   in the order they are found.  It never shrinks. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the sum of distinct prime divisors of x.  We treat x as an
   unsigned 64‑bit integer, but the result is held in a 64‑bit unsigned
   integer to avoid overflow during the sum. */
static uint64_t sum_distinct_primes(uint64_t x)
{
    uint64_t sum = 0;
    update_spf(x);
    while (x > 1) {
        uint64_t p = (uint64_t)spf[x];
        sum += p;
        while (x % p == 0) x /= p;
    }
    return sum;
}

/* Main sequence function as requested. */
seq_t a(idx_t n)
{
    /* Find terms until we have at least n+1 of them. */
    while (cache_len <= (size_t)n) {
        /* Use a 64‑bit variable for the test value.  We keep the
           next integer to test in a static variable so that calls
           are efficient regardless of call order. */
        static idx_t next_to_test = 2;   /* first candidate is 2 */

        uint32_t pop = popcount_uint64(next_to_test);
        uint64_t sump = sum_distinct_primes(next_to_test);

        if (sump % pop == 0) {
            /* Term found; store it in the cache. */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)next_to_test;
        }

        ++next_to_test;
    }

    return cache[(size_t)n];
}
