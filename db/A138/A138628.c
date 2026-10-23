/*
 * A138628  Positive integers k such that 17*k-7 is prime.
 * https://oeis.org/A138628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 525, u32 n <= 55, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138628: a(n) = smallest positive integer k such that 17*k-7 is prime. */
/* The function caches found values and grows as needed. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of found k values (cache[0] = a(1)). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    /* Next k to test. */
    static idx_t next_k = 1;

    /* Allocate initial cache if it does not exist yet. */
    if (cache == NULL)
    {
        cache_cap = 1024;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0;          /* allocation failed – return 0 as a sentinel */
    }

    /* Grow cache until we have the requested number of terms. */
    while (cache_size < n)
    {
        /* Compute candidate value 17*k-7 in an integer type able to hold the result. */
        unsigned __int128 cand128 = (unsigned __int128)17 * next_k - 7;
        seq_t cand = (seq_t)cand128;        /* cast to seq_t before primality test */

        if (is_prime_big(cand))
        {
            /* Ensure capacity. */
            if (cache_size == cache_cap)
            {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp)
                {
                    free(cache);
                    cache = NULL;
                    return (seq_t)0;      /* allocation failed */
                }
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        next_k++;
    }

    return cache[n - 1];
}
