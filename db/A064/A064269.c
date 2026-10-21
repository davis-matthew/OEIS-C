/*
 * A064269  Numbers k such that prime(k) - k is prime.
 * https://oeis.org/A064269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 582, u32 n <= 583, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064269: numbers k such that prime(k) – k is prime.
   The implementation keeps a cache of qualifying indices and
   extends a prime sieve on demand.  The function is thread‑safe
   for read‑only use but does not provide reentrancy.  It works
   for any build of seq_t (fixed‑size or arbitrary‑precision). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/is_prime_big.h"       /* is_prime_big */

static idx_t *cache      = NULL;   /* cached qualifying k values (1‑based) */
static size_t cache_len  = 0;      /* number of cached terms */
static size_t cache_cap  = 0;      /* allocated capacity */

static uint64_t *prime_list = NULL;   /* primes up to prime_limit */
static size_t  prime_cnt   = 0;       /* number of primes in prime_list */
static idx_t   prime_limit = 0;       /* current upper bound for sieve */
static idx_t   last_checked_k = 0;    /* index of last examined prime (0‑based) */

/* Ensure that cache contains at least 'target' terms (target >= 1). */
static void extend_cache_if_needed(idx_t target)
{
    while (cache_len < target) {
        /* Expand the prime sieve if needed. */
        if (last_checked_k >= prime_cnt) {
            /* Determine next sieve limit. */
            if (prime_limit == 0)
                prime_limit = 200000U;          /* initial bound */
            else
                prime_limit *= 2;                /* double each time */

            if (prime_list)
                free(prime_list);
            build_primes_upto(prime_limit, &prime_list, &prime_cnt);
        }

        /* Scan new primes and collect qualifying indices. */
        for ( ; last_checked_k < prime_cnt && cache_len < target; ++last_checked_k) {
            idx_t k    = last_checked_k + 1;            /* 1‑based index */
            uint64_t p = prime_list[last_checked_k];    /* prime(k) */

            seq_t diff = (seq_t)p - (seq_t)k;           /* prime(k) – k */
            if (diff > (seq_t)0 && is_prime_big(diff)) {
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap ? cache_cap * 2 : 16);
                    cache = (idx_t *)realloc(cache, cache_cap * sizeof(idx_t));
                }
                cache[cache_len++] = k;
            }
        }
    }
}

/* Return the nth term (1‑based) of A064269. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* undefined for n <= 0 */
    extend_cache_if_needed(n);
    return (seq_t)cache[n - 1];
}
