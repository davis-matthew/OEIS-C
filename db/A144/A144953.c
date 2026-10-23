/*
 * A144953  Primes of form n^3 + 2.
 * https://oeis.org/A144953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A067200(n)^3 + 2 (primes of the form k³ + 2, k starting
 *            at 0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 117, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes (in order of appearance). */
    static seq_t *cache = NULL;   /* array of primes */
    static size_t cache_size = 0; /* number of primes cached */
    static size_t cache_cap  = 0; /* allocated capacity            */
    static idx_t next_k      = 0; /* next integer k to test        */

    /* Extend the cache until we have at least n primes. */
    while (cache_size < n)
    {
        /* Candidate = k³ + 2, using seq_t arithmetic. */
        seq_t k        = (seq_t)next_k;
        seq_t candidate = k * k * k + (seq_t)2;

        /* Test primality of the candidate. */
        if (isPrimeGeneric(candidate))
        {
            /* Ensure sufficient capacity. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}
