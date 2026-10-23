/*
 * A142904  Primes congruent to 26 mod 63.
 * https://oeis.org/A142904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime ≡ 26 (mod 63)
 * Range:     u8 n <= 1, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found primes in the progression 26 mod 63 */
    static seq_t *cache = NULL;   /* Array of primes */
    static size_t cache_cap = 0;  /* Allocated capacity */
    static idx_t  cache_len = 0;  /* Number of primes stored */
    static idx_t  m = 0;          /* Next k to test: candidate = 26 + 63*m */

    /* Keep filling the cache until it contains at least n primes */
    while (cache_len < n)
    {
        /* Compute next candidate in the progression */
        idx_t candidate = 26 + 63 * m;
        m++; /* Advance for next time */

        /* Test for primality */
        if (is_prime(candidate))
        {
            /* Ensure enough capacity */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure, return 0 as sentinel */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the prime as seq_t */
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    /* Return the requested prime */
    return cache[n - 1];
}
