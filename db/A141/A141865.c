/*
 * A141865  Primes congruent to 13 mod 17.
 * https://oeis.org/A141865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 405, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime p such that p ≡ 13 (mod 17) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */

    /* Static cache of computed terms. */
    static seq_t *cache       = NULL;
    static size_t  cache_cap  = 0;
    static idx_t   cache_len  = 0;
    /* Next integer from which to search for the next prime. */
    static uint64_t next_start = 13;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        seq_t p;
        uint64_t p64;

        /* Handle the first prime 13 specially, because next_prime
           returns the *next* prime > its argument.  */
        if (next_start == 13)
        {
            p   = (seq_t)13;
            p64 = 13;
            next_start = 14;          /* start future searches from 14 */
        }
        else
        {
            /* Find the next prime ≥ next_start. */
            p64 = next_prime(next_start);
            p   = (seq_t)p64;
            next_start = p64 + 1;     /* avoid re‑testing this prime */
        }

        /* If it satisfies the congruence, store it. */
        if ((uint64_t)p % 17 == 13)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);           /* minimal error handling */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = p;
        }
    }

    return cache[n - 1];
}
