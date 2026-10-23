/*
 * A124584  Primes p such that q-p >= 10, where q is the next prime after p.
 * https://oeis.org/A124584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS sequence A124584:
 * Primes p for which the next prime q after p satisfies q - p ≥ 10.
 * The function builds a static cache of qualifying primes on demand.
 * It uses the helper function `next_prime(uint64_t start)` from
 * "../utils/next_prime.h" to advance through primes.
 *
 * For idx_t = uint64_t and seq_t an unsigned integer type or an
 * arbitrary‑precision integer class, the implementation casts
 * intermediate 64‑bit values to seq_t when storing them.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"   /* decl: uint64_t next_prime(uint64_t start) */

seq_t a(idx_t n)
{
    /* static cache of qualifying primes */
    static seq_t *cache      = NULL;
    static size_t cache_cnt  = 0;   /* number of stored primes in cache */
    static size_t cache_cap  = 0;   /* allocated capacity of cache */
    static uint64_t last_checked = 0; /* last prime examined */

    /* The sequence is defined for n ≥ 1 (offset = 1). Behaviour for n = 0 is
     * irrelevant but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Grow the cache until it contains at least n elements. */
    while (cache_cnt < (size_t)n)
    {
        /* Find next prime after last_checked. */
        uint64_t p = next_prime(last_checked + 1);
        uint64_t q = next_prime(p + 1);

        /* Check the gap condition. */
        if (q - p >= 10U)
        {
            /* Ensure there is room in the cache. */
            if (cache_cnt == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    return 0;           /* allocation failure → return 0 */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            /* Store the qualifying prime.  Implicit conversion to seq_t
             * is allowed for all given seq_t types. */
            cache[cache_cnt++] = (seq_t)p;
        }

        /* Set last_checked so that next iteration starts after p. */
        last_checked = p;
    }

    /* Return the (n‑1)th element of the cache (zero‑based index). */
    return cache[n - 1];
}
