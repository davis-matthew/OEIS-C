/*
 * A141914  Primes congruent to 10 mod 23.
 * https://oeis.org/A141914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 301, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 10 (mod 23). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Static cache of primes found so far. */
    static seq_t *cache      = NULL;
    static size_t size       = 0;      /* number of cached primes */
    static size_t capacity   = 0;      /* allocated cache size   */
    static uint64_t next     = 79;     /* first prime ≡10 (mod 23) */

    if (n == (idx_t)0)          /* undefined for n=0, return 0 */ 
        return (seq_t)0;

    /* Return cached value if already computed. */
    if ((size_t)n <= size)
        return cache[(size_t)n - 1U];

    /* Generate primes until we have at least n of them. */
    while ((size_t)n > size)
    {
        if (is_prime((idx_t)next))
        {
            /* Grow cache if necessary. */
            if (size == capacity)
            {
                size_t new_cap = capacity ? capacity << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();   /* out of memory – stop execution */
                cache = tmp;
                capacity = new_cap;
            }
            cache[size++] = (seq_t)next;   /* store prime */
        }
        next += 23;        /* next candidate ≡10 (mod 23) */
    }

    return cache[(size_t)n - 1U];
}
