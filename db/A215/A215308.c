/*
 * A215308  Primes congruent to {1, 2, 3, 5} mod 17.
 * https://oeis.org/A215308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215308: primes congruent to 1, 2, 3, or 5 modulo 17.
   The function tests successive integers for primality using
   the provided is_prime_small helper and collects those with the
   required congruence.  A cache of primes is maintained so that
   subsequent calls, in any order, can be answered efficiently. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static state holding the cache and the next integer to check */
    static uint64_t current = 2;           /* next number to test */
    static seq_t *cache = NULL;           /* array of qualifying primes */
    static size_t cache_len = 0;           /* number of primes stored */
    static size_t cache_cap = 0;           /* allocated capacity of cache */

    /* Expand the cache until it contains at least n primes. */
    while (cache_len < (size_t)n)
    {
        if (is_prime_small((seq_t)current))
        {
            uint64_t r = current % 17;
            if (r == 1 || r == 2 || r == 3 || r == 5)
            {
                /* Allocate or grow the cache if necessary. */
                if (cache_len == cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* allocation failed */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)current;
            }
        }
        /* Proceed with the next candidate. */
        current++;
    }

    return cache[(size_t)(n - 1)];
}
