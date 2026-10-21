/*
 * A215282  Primes congruent to {2, 3, 4} mod 19.
 * https://oeis.org/A215282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n-th prime that is congruent to
 * 2, 3 or 4 (mod 19).  OEIS offset 1, so a(1) = 2.
 *
 * It is implemented by maintaining a cache of primes found so far.
 * New primes are searched for by simple trial division using the
 * supplied helper is_prime_small.  The cache is grown
 * geometrically and can be requested in any order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

static seq_t *prime_cache = NULL;   /* cached primes */
static size_t  cache_count = 0;     /* number of primes cached */
static size_t  cache_cap   = 0;     /* allocated capacity */

/* Allocate or grow the prime cache to hold at least need elements */
static void ensure_prime_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* out‑of‑memory */
    prime_cache = new_arr;
    cache_cap   = new_cap;
}

/* Return the n-th prime congruent to 2, 3 or 4 modulo 19
   (OEIS offset 1).  For n == 0 a(0) is undefined; we return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    size_t idx = (size_t)n;   /* safe: n ≥ 1, idx tiny enough */

    /* Search for more primes until we have at least idx primes cached. */
    if (cache_count < idx) {
        uint64_t candidate = 2;
        if (cache_count > 0) candidate = (uint64_t)prime_cache[cache_count - 1] + 1;

        while (cache_count < idx) {
            /* Find next prime ≥ candidate.  Simple incremental search. */
            while (!is_prime_small(candidate)) candidate++;

            /* Check the modular congruence. */
            uint64_t r = candidate % 19ULL;
            if (r == 2ULL || r == 3ULL || r == 4ULL) {
                ensure_prime_capacity(cache_count + 1);
                prime_cache[cache_count] = (seq_t)candidate;
                cache_count++;
            }
            candidate++;   /* next candidate for the next loop */
        }
    }

    return prime_cache[idx - 1];
}
