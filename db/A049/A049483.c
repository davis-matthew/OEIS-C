/*
 * A049483  Primes p such that p + 2310 is also prime, where 2310 is the 5th
 *          primorial number A002110(5).
 * https://oeis.org/A049483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2386, u32 n <= 2628, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p for which p + 2310 is also prime.
   It uses a cached list of previously found terms and expands it only
   enough to satisfy each request.  The search proceeds by iterating over
   consecutive primes `p` (using `next_prime`) and testing primality of
   `p + 2310` with `is_prime_big`.  The sequence is defined for n ≥ 1, so
   a(1) = 23, a(2) = 29, … . */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n)       */
#include "../utils/next_prime.h"     /* uint64_t next_prime(uint64_t)   */

/* --- static state ----------------------------------------------------- */
static seq_t  *cache      = NULL;   /* stored terms of the sequence   */
static size_t  cache_sz   = 0;      /* number of stored terms          */
static size_t  cache_cap  = 0;      /* allocated capacity              */
static uint64_t next_candidate = 2; /* next prime to examine           */
static bool     first_call     = true; /* flag for first initialization  */

/* --- helper ------------------------------------------------------------ */
static void fill_cache(idx_t n)
{
    /* Ensure the cache holds at least n terms.  We keep generating
       primes until we find the required number of terms. */
    while (cache_sz < n) {
        if (first_call) {
            /* First call: initialise the candidate to 2 */
            next_candidate = 2;
            first_call     = false;
        }

        uint64_t p = next_candidate;                /* current prime */
        /* Test whether p + 2310 is prime.  The sum fits into seq_t
           because for the range of n we consider it stays far below
           the limits of seq_t. */
        if (is_prime_big((seq_t)(p + 2310U))) {
            /* Grow the cache if needed */
            if (cache_sz == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) {
                    /* Allocation failure: abort the program. */
                    abort();
                }
            }
            cache[cache_sz++] = (seq_t)p;
        }

        /* Advance to the next prime candidate. */
        next_candidate = next_prime(p + 1);
    }
}

/* --- required public function ------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based; ensure we have at least n values */
    fill_cache(n);
    return cache[n - 1];
}
