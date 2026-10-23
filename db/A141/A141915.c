/*
 * A141915  Primes congruent to 11 mod 23.
 * https://oeis.org/A141915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 11 (mod 23)
 * Range:     u8 n <= 4, u16 n <= 258, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generator for OEIS A141915.
   It uses a simple incremental search with a cache so that calls
   can come in any order.  The built-in primality test
   is_prime_big is available in the utilities and works for all
   supported integer types. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;
    static seq_t  next_candidate;
    static bool   init       = false;

    /* one-time initialization */
    if (!init) {
        init = true;
        cache_cap          = 64;
        cache              = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        next_candidate     = (seq_t)11;   /* first candidate 11 ≡ 11 (mod 23) */
    }

    /* generate primes until the cache contains at least n primes */
    while (cache_len < n) {
        seq_t cand = next_candidate;

        /* test for primality */
        if (is_prime_big(cand)) {
            /* store it in the cache */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }

        /* advance to the next number ≡ 11 (mod 23) */
        next_candidate = cand + (seq_t)23;
    }

    return cache[n - 1];
}
