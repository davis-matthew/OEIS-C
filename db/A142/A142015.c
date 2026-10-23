/*
 * A142015  Primes congruent to 11 mod 31.
 * https://oeis.org/A142015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 217, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th prime congruent to 11 modulo 31 (OEIS A142015) */
/* The implementation uses a prime sieve (build_primes_upto) and keeps a static cache
   of all primes ≡ 11 (mod 31) that have been found so far.  No hard‑coded list
   is used; the sequence is generated on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

/* Cached primes that are ≡ 11 (mod 31) */
static uint64_t *prime_cache = NULL;   /* array of uint64_t primes */
static size_t   cache_len   = 0;       /* number of cached primes          */
static size_t   cache_cap   = 0;       /* current allocation capacity      */

/* Highest prime limit that has already been sieved using build_primes_upto */
static idx_t   sieve_limit = 0;

/* Reallocate the cache to hold at least new_cap 64‑bit prime entries */
static void cache_grow(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp && new_cap > 0) abort();            /* out of memory */
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Ensure that prime_cache has at least n primes ≡ 11 (mod 31) */
static void ensure_cache(size_t n)
{
    if (cache_len >= n) return;                 /* already enough */

    /* Start sieving from the current limit; double until we have enough terms */
    idx_t limit = (sieve_limit > 0) ? sieve_limit * 2 : 1000;
    if (limit < 1000) limit = 1000;

    while (cache_len < n) {
        /* Sieve primes up to `limit` */
        uint64_t *primes  = NULL;
        size_t    pcnt    = 0;
        build_primes_upto(limit, &primes, &pcnt);

        /* Append new primes that are > sieve_limit (i.e., new primes) */
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            if (p <= (uint64_t)sieve_limit) continue;      /* already processed */
            if (p % 31ULL == 11ULL) {
                /* Grow cache if necessary */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    cache_grow(new_cap);
                }
                prime_cache[cache_len++] = p;
            }
        }
        free(primes);

        /* Update the sieve limit for the next iteration */
        sieve_limit = limit;
        if (cache_len >= n) break;          /* we now have enough primes */

        limit <<= 1;                        /* double the limit for next pass */
    }
}

/* Return the n‑th term of the sequence (OEIS offset is 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n == 0 */
    ensure_cache((size_t)n);
    return (seq_t)prime_cache[(size_t)n - 1];
}
