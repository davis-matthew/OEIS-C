/*
 * A142768  Primes congruent to 41 mod 59.
 * https://oeis.org/A142768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 41 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1) */

#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;          /* array of found primes */
static size_t  cap   = 0;            /* capacity of cache array   */
static size_t  count = 0;            /* number of filled elements  */
static seq_t   next_candidate = 41;  /* first candidate to test   */

seq_t a(idx_t n)
{
    /* Return the n-th prime, 1‑based indexing. */
    /* Grow the cache until it contains at least n elements. */
    size_t needed = (size_t)n;
    if (needed > count)
    {
        if (cache == NULL)
        {
            /* initialise cache */
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache) abort();
            next_candidate = 41;
        }
        /* Test candidates until we have enough primes. */
        while (count < needed)
        {
            /* Check if current candidate is prime. */
            if (isPrimeGeneric(next_candidate))
            {
                /* Ensure capacity. */
                if (count == cap)
                {
                    size_t newcap = cap * 2;
                    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!newptr) abort();
                    cache = newptr;
                    cap   = newcap;
                }
                cache[count++] = next_candidate;
            }
            next_candidate += 59;   /* Next number ≡ 41 (mod 59) */
        }
    }
    /* Return the cached result. */
    return cache[needed - 1];
}
