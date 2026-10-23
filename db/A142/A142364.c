/*
 * A142364  Primes congruent to 13 mod 47.
 * https://oeis.org/A142364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 13 modulo 47.
   a(n) is the nth prime p such that p % 47 == 13.
   The function computes primes incrementally and caches them
   so that calls in any order are fast. */
#include <stddef.h>
#include "../utils/is_prime.h"

/* Static cache of found primes.  The cache is large enough for
   the first 200 000 primes in this progression, far beyond the
   10 000‑term requirement. */
#define CACHE_CAP 200000
static seq_t  cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t  next_candidate = 13;    /* first number ≡13 (mod 47) */

/* Return the nth prime ≡13 (mod 47).  OEIS offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* harmless for unknown callers */
        return 0;

    /* Generate primes until the cache contains at least n elements. */
    while (cache_len < n) {
        /* Find the next prime in the arithmetic progression. */
        while (true) {
            if (is_prime(next_candidate)) {
                if (cache_len < CACHE_CAP)
                    cache[cache_len++] = (seq_t)next_candidate;
                /* prime found, advance to the next candidate. */
                break;
            }
            next_candidate += 47;
        }
        /* The candidate currently holds the prime we just stored.
           Advance to the next candidate in the progression. */
        next_candidate += 47;
    }

    return cache[n - 1];
}
