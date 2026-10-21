/*
 * A054812  Fifth term of strong prime quintets: p(m-3)-p(m-4) > p(m-2)-p(m-3)
 *          > p(m-1)-p(m-2) > p(m)-p(m-1).
 * https://oeis.org/A054812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A054812 by scanning primes for strictly
   decreasing gaps of length 5 (strong prime quintets).  The function
   builds primes incrementally until the requested count is reached.
   It caches the results for future calls.

   Requirements:
   - External linkage only: seq_t a(idx_t n)
   - Uses the build_primes utility for prime generation.
   - Compatible with both C11 and C++17 (seq_t may be an arbitrary-precision
     class; casting to (seq_t*) is acceptable).
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* ----- static data for caching ------------------------------------------------ */
static idx_t   *primes       = NULL;  /* array of primes up to current limit */
static size_t   prime_cnt    = 0;     /* number of primes in the array */
static seq_t   *cache        = NULL;  /* array of a(n) values found so far */
static size_t   cache_len    = 0;     /* number of entries stored in cache */
static size_t   cache_cap    = 0;     /* capacity of cache array             */
static bool     initialized  = false; /* true after first successful build   */
static idx_t    cur_limit    = 1000000; /* starting sieve limit                */

/* ----- helper: build cache up to at least the nth term ------------------------ */
static void ensure_cache(idx_t n)
{
    /* If we already have enough terms, nothing to do */
    if (initialized && cache_len >= (size_t)n)
        return;

    idx_t limit = cur_limit;
    while (true) {
        /* Free previous data if any */
        if (primes) free(primes);
        if (cache) free(cache);

        /* Build primes up to current limit */
        build_primes(limit, &primes, &prime_cnt);

        /* Prepare a fresh cache */
        cache_cap = 1024;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        /* Scan for strict decreasing gaps of length 5 */
        for (size_t i = 4; i < prime_cnt; ++i) {
            uint64_t d1 = primes[i-3] - primes[i-4];
            uint64_t d2 = primes[i-2] - primes[i-3];
            uint64_t d3 = primes[i-1] - primes[i-2];
            uint64_t d4 = primes[i]   - primes[i-1];

            if (d1 > d2 && d2 > d3 && d3 > d4) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)primes[i];
            }
        }

        if (cache_len >= (size_t)n) {
            initialized = true;
            cur_limit   = limit;
            return; /* sufficient terms collected */
        }

        /* Not enough terms – double the search limit and try again */
        limit *= 2;
    }
}

/* ----- public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence is 1‑based; a(0) undefined */
    ensure_cache(n);
    if (cache_len < (size_t)n) return 0; /* should not happen */
    return cache[n - 1];
}
