/*
 * A142576  Primes congruent to 46 mod 53.
 * https://oeis.org/A142576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 118, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth prime congruent to 46 modulo 53.
   Computed by generating candidates of the form 53*k+46 and testing
   primality with the helper `is_prime_big`. */

/* Required helper */
#include "../utils/is_prime_big.h"

#include <stdbool.h>
#include <stdlib.h>

/* Static cache for computed primes */
static seq_t *cache = NULL;          /* array of primes */
static idx_t cache_size = 0;         /* number of primes stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t next_k = 5;             /* 53*next_k + 46 is the next candidate */

/* Return the nth element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n < 1 is irrelevant. */
    if (n == 0)
        return 0;

    /* Expand the cache until it contains at least n primes. */
    while (cache_size < n)
    {
        /* Candidate prime of the form 53*k + 46 */
        seq_t cand = (seq_t)53 * (seq_t)next_k + (seq_t)46;

        /* Test primality using the provided helper. */
        if (is_prime_big(cand))
        {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);   /* Out of memory – terminate. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;   /* store the found prime */
        }
        ++next_k;   /* advance to the next candidate */
    }

    /* Sequence is 1‑based. */
    return cache[n - 1];
}
