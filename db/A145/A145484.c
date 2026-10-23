/*
 * A145484  Primes p such that 2*p - 29 is a positive prime.
 * https://oeis.org/A145484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 594, u32 n <= 50, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 2*p-29 is a positive prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;       /* cached a(k) values */
    static size_t cache_size = 0;     /* number of cached terms */
    static size_t cache_cap  = 0;     /* allocated capacity */

    static idx_t *prime_list = NULL;  /* list of primes */
    static size_t prime_cnt  = 0;     /* number of primes in the list */
    static idx_t prime_idx  = 0;      /* next prime to examine */

    static bool initialised = false;

    /* Helper: grow cache if needed */
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise once: generate a first prime list */
    if (!initialised) {
        idx_t limit = 500000;   /* first sieve limit */
        build_primes(limit, &prime_list, &prime_cnt);
        initialised = true;
    }

    /* expand prime list until we have enough terms */
    while (cache_size < (size_t)n) {
        /* If we've exhausted current primes, enlarge the sieve */
        if (prime_idx >= prime_cnt) {
            idx_t old_limit  = prime_list[prime_cnt - 1] + 1;
            idx_t new_limit  = old_limit * 2;
            free(prime_list);
            build_primes(new_limit, &prime_list, &prime_cnt);
            /* prime_idx already points to the next new prime */
        }

        /* take next prime p from the list */
        seq_t p = (seq_t)prime_list[prime_idx++];
        /* compute q = 2*p - 29 */
        seq_t q = (seq_t)2 * p - (seq_t)29;

        /* test if q is a positive prime */
        if (q > (seq_t)0 && is_prime_big(q)) {
            /* store p into cache */
            cache[cache_size++] = p;
        }

        /* ensure cache capacity for next insertion if needed */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    return cache[n - 1];
}
