/*
 * A142144  Primes congruent to 35 mod 37.
 * https://oeis.org/A142144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 180, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 35 mod 37.
   The function searches numbers n ≡ 35 (mod 37) in increasing order
   and tests each for primality using the provided is_prime_big routine.
   Results are cached so that subsequent calls are fast. */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of previously found primes in this progression. */
    static seq_t *cache   = NULL;
    static idx_t len      = 0;   /* number of primes stored */
    static idx_t cap      = 0;   /* current capacity of the cache */
    static seq_t last_cand = (seq_t)35;   /* last number tested (≡35 mod 37) */

    /* Expand the cache until it contains at least n primes. */
    while (len < n)
    {
        /* Advance to the next candidate: add 37. */
        last_cand += (seq_t)37;

        /* Test the candidate for primality. */
        if (is_prime_big(last_cand))
        {
            /* Grow the cache if necessary. */
            if (len == cap)
            {
                cap = cap ? cap * 2 : 32;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[len++] = last_cand;
        }
    }

    return cache[n - 1];
}
