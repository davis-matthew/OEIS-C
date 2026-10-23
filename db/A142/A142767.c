/*
 * A142767  Primes congruent to 40 mod 59.
 * https://oeis.org/A142767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 40 mod 59 (computed by incremental search) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p such that p % 59 == 40.
   Indexes start at 1 (a(1)=571, a(2)=1279, …). */
seq_t a(idx_t n)
{
    /* For any call with n < 1 we return 0 (out of offset). */
    if (n == 0)
        return 0;

    /* Static cache for previously found terms. */
    static seq_t *cache      = NULL;  /* array of primes  */
    static size_t  cache_cap = 0;      /* allocated capacity */
    static idx_t   cache_cnt = 0;      /* number of terms stored */

    /* Current candidate in the arithmetic progression 40 + 59*k. */
    static uint64_t current = 571;     /* first prime ≡ 40 (mod 59) */

    /* Grow cache until we have at least n terms. */
    while (cache_cnt < n)
    {
        /* Test if current number is prime. */
        if (is_prime((idx_t)current))
        {
            /* Allocate or enlarge the cache if necessary. */
            if (cache == NULL)
            {
                cache_cap = 16;
                cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
                if (!cache)
                    return 0;   /* allocation failure */
            }
            else if ((size_t)cache_cnt == cache_cap)
            {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp)
                    return 0;   /* allocation failure */
                cache = tmp;
            }

            /* Store the found prime. */
            cache[cache_cnt++] = (seq_t)current;
        }

        /* Move to the next number of the progression. */
        current += 59;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
