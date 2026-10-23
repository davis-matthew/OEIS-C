/*
 * A181606  Twin primes ending in 9.
 * https://oeis.org/A181606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 566, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = nth prime ending in 9 that is part of a twin prime pair.
 * The sequence is computed via a simple incremental sieve that checks
 * primes ending in 9 and their neighbours for primality.
 * The function memoises results so that any order of calls is handled.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *cache   = NULL;      /* array of found terms          */
    static idx_t cache_len = 0;         /* number of stored terms        */
    static idx_t cache_cap = 0;         /* allocated capacity (in terms) */
    static idx_t next_candidate = 19;   /* next number ending in 9 to test */

    /* Extend the cache until we have at least n terms */
    while (cache_len < n)
    {
        /* Find the next prime ending in 9 that is in a twin prime pair */
        while (true)
        {
            /* Ensure candidate is prime */
            if (!is_prime(next_candidate))
            {
                next_candidate += 10;
                continue;
            }

            /* Check if the candidate is part of a twin prime pair */
            if (is_prime(next_candidate - 2) || is_prime(next_candidate + 2))
                break;   /* candidate is a valid term */

            next_candidate += 10;
        }

        /* Grow the cache array if necessary */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();                     /* should not happen with small limits */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the found term */
        cache[cache_len++] = (seq_t)next_candidate;

        /* Advance to the next candidate ending in 9 */
        next_candidate += 10;
    }

    return cache[n - 1];
}
