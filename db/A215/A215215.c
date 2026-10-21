/*
 * A215215  Primes congruent to {0, 1, 2, 5} mod 13.
 * https://oeis.org/A215215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th prime that is congruent to
   0, 1, 2 or 5 modulo 13. It is implemented by incrementally
   testing successive integers and caching the found primes.
   This approach works for the required range (n ≲ 10000)
   in well under two seconds. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Return 0 for the undefined offset 0. */
    if (n == 0) return 0;

    /* static state for caching */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_checked = 1; /* next integer to examine */

    /* Grow the cache until it contains at least n values. */
    while (cache_size < n)
    {
        /* examine the next integer */
        ++last_checked;

        /* select the residue classes 0,1,2,5 mod 13 */
        idx_t r = last_checked % 13;
        if (!(r == 0 || r == 1 || r == 2 || r == 5))
            continue;

        /* test primality */
        if (is_prime_small((seq_t)last_checked))
        {
            /* enlarge the cache if necessary */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* unrecoverable */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
