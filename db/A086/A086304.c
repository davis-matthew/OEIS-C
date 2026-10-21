/*
 * A086304  Numbers n such that n+6 is prime.
 * https://oeis.org/A086304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n - 6 where p_n is the n‑th prime ≥ 7 (equivalently,
 *            numbers n such that n+6 is prime)
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache of returned values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of elements stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* current prime being processed (initialized to the first prime ≥ 7) */
    static uint64_t cur_prime = 7;
    static int initialized = 0;

    if (!initialized)
    {
        /* start with a modest capacity; grow as needed */
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
        initialized = 1;
    }

    /* Generate further candidates until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        /* store the candidate (prime - 6) */
        seq_t val = (seq_t)(cur_prime - 6);

        /* grow the buffer if necessary */
        if (cache_len >= cache_cap)
        {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }

        cache[cache_len++] = val;

        /* advance to the next prime */
        cur_prime = next_prime(cur_prime + 1);
    }

    /* a(1) corresponds to cache[0] */
    return cache[n - 1];
}
