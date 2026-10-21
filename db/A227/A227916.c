/*
 * A227916  Primes that remain prime when the leftmost digit is removed.
 * https://oeis.org/A227916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A227916: primes that remain prime when the leftmost
 * digit is removed.  The function builds the sequence by searching
 * successive primes, testing the truncated value, and caching the
 * qualifying primes.  The cache allows arbitrary order of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static seq_t *cache      = NULL;   /* cached qualifying primes   */
static size_t cache_cap  = 0;      /* capacity of cache          */
static size_t cache_len  = 0;      /* number of cached primes    */
static uint64_t last_tested = 1;   /* last candidate processed   */

/* Grow the cache array when its capacity is exhausted. */
static void cache_grow(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *newp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (newp == NULL) abort();             /* out of memory */
    if (cache && cache_cap) {
        memcpy(newp, cache, cache_len * sizeof(seq_t));
    }
    free(cache);
    cache      = newp;
    cache_cap  = new_cap;
}

/* Return the n-th prime (n >= 1) that remains prime after removing its
 * leftmost digit.  For n == 0 a sentinel value 0 is returned. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined at offset 0 */

    /* Generate qualifying primes until the cache holds at least n. */
    while (cache_len < (size_t)n) {
        /* Search for the next suitable prime starting from the last
           candidate checked. */
        for (uint64_t candidate = last_tested; ; ++candidate) {
            /* Skip numbers < 2. */
            if (candidate < 2) continue;

            /* Fast primality test for 64‑bit integers. */
            if (!is_prime(candidate)) continue;

            /* Compute 10^(d-1) where d is the number of decimal digits
             * of candidate, and then the truncated tail. */
            uint64_t pow10 = 1;
            while (candidate / pow10 >= 10) pow10 *= 10;
            uint64_t tail = candidate % pow10;

            /* tail must be prime (and > 1). */
            if (tail >= 2 && is_prime(tail)) {
                /* Store the qualifying prime. */
                if (cache_len == cache_cap) cache_grow();
                cache[cache_len++] = (seq_t)candidate;

                /* Update the next starting point and exit the search. */
                last_tested = candidate + 1;
                break;
            }
        }
    }

    return cache[n - 1];
}
