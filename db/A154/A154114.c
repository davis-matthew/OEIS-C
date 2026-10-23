/*
 * A154114  Primes p such that p + 9699690 is also prime, where 9699690 is the
 *          8th primorial number A002110(8).
 * https://oeis.org/A154114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th prime p such that p + 9699690 is also prime.
 * (9699690 = 2·3·5·7·11·13·17·19, the 8th primorial). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------- */
/*                                                                    */
seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0.  Returning 0 prevents UB. */
    if (n == 0)
        return (seq_t)0;

    /* ---------------------------------------------------------------- */
    /* Static cache for found terms.                                   */
    static seq_t *cache     = NULL;   /* array of seq_t values          */
    static size_t cache_len = 0;     /* current number of terms stored */
    static size_t cache_cap = 0;     /* current capacity (in elements) */

    /* ---------------------------------------------------------------- */
    /* Static sieve data.                                               */
    static uint64_t *primes            = NULL;   /* prime table              */
    static size_t    primes_cnt       = 0;      /* number of stored primes  */
    static idx_t     last_prime_index = 0;      /* next prime to examine    */
    static idx_t     sieve_limit     = 0;      /* current sieve size       */

    /* Initialise cache. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* safety */
    }

    /* Initialise sieve (first call). */
    if (primes == NULL) {
        sieve_limit = 2000000;                  /* start with 2 M */
        build_primes_upto(sieve_limit, &primes, &primes_cnt);
    }

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* If we ran out of primes in the current sieve, double it. */
        if (last_prime_index >= primes_cnt) {
            idx_t new_limit = sieve_limit * 2;
            uint64_t *new_primes = NULL;
            size_t new_cnt = 0;
            build_primes_upto(new_limit, &new_primes, &new_cnt);
            free(primes);
            primes    = new_primes;
            primes_cnt = new_cnt;
            sieve_limit = new_limit;
            /* last_prime_index already points just past the old array,
               which is now valid after reallocation. */
        }

        /* Scan remaining primes in the current sieve. */
        while (cache_len < (size_t)n && last_prime_index < primes_cnt) {
            uint64_t p = primes[last_prime_index++];
            /* Check primality of p and p+9699690. */
            if (is_prime_big((seq_t)p) &&
                is_prime_big((seq_t)(p + 9699690))) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache,
                           cache_cap * sizeof(seq_t));
                    if (tmp == NULL) return (seq_t)0; /* safety */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }
    }

    return cache[n - 1];
}
