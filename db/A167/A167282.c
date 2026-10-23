/*
 * A167282  Primes containing 777 as a substring.
 * https://oeis.org/A167282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_777, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence of primes whose decimal representation contains
   the substring "777".  a(n) is the nth such prime (a(1)=1777).  The
   implementation generates successive primes using the utility
   next_prime(uint64_t) and keeps only those that contain 777.  A
   static cache of found primes is maintained so that calls in any
   order are answered in O(1) after the required primes have been
   generated. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"
#include "../utils/contains_777.h"

/* Static cache for the sequence. */
static seq_t *prime_cache      = NULL;   /* array of primes */
static size_t cache_len        = 0;      /* number of cached primes */
static size_t cache_cap        = 0;      /* allocated capacity */
static uint64_t next_start     = 2;      /* next value to search for a prime */

/* Return the nth term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Generate primes until we have at least n terms cached. */
    while (cache_len < (size_t)n)
    {
        /* Get the next prime after next_start. */
        uint64_t cand = next_prime((uint64_t)next_start);
        if (cand == 0) break;            /* should not happen */

        /* Advance the starting point for the next search. */
        next_start = cand + 1;

        /* Convert candidate to seq_t for the substring test. */
        seq_t pcand = (seq_t)cand;

        if (contains_777(pcand))
        {
            /* Grow cache if needed. */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
                prime_cache = (seq_t *)realloc(prime_cache,
                                               cache_cap * sizeof(seq_t));
                if (!prime_cache) exit(EXIT_FAILURE); /* allocation failure */
            }
            prime_cache[cache_len++] = pcand;
        }
    }
    return prime_cache[(size_t)n - 1];
}
