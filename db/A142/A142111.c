/*
 * A142111  Primes congruent to 35 mod 36.
 * https://oeis.org/A142111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 418, u32 n <= 543, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime that is congruent to 35 mod 36.  */
/* Uses a simple incremental sieve: test each candidate 35 mod 36
   for primality and store the ones that succeed.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Memoization storage.  */
    static seq_t *cache   = NULL;    /* array of found primes */
    static size_t cache_cap = 0;     /* current capacity of cache   */
    static idx_t  cache_len = 0;     /* number of primes actually stored */
    static uint64_t candidate = 71;  /* first prime ≡35 (mod 36)      */

    /* If we already have the requested term, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate primes until we reach n terms. */
    while (cache_len < n)
    {
        /* Test the current candidate. */
        if (is_prime_big((seq_t)candidate))
        {
            /* Grow the cache if needed. */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        /* Advance to the next integer congruent to 35 mod 36. */
        candidate += 36;
    }

    return cache[n - 1];
}
