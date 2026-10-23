/*
 * A139405  Numbers k such that 8*k+1 and 8*k+7 are primes.
 * https://oeis.org/A139405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k such that 8*k+1 and 8*k+7 are primes.
   The function finds the terms by brute‑force search and caches the results
   so that subsequent calls are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;        /* cached a(n) values */
static size_t cache_cap = 0;       /* capacity of cache array */
static idx_t next_k = 1;           /* next k to test */
static idx_t cache_len = 0;        /* number of values stored */

/* ensure the cache has room for at least new_len entries */
static void ensure_cache_capacity(size_t new_len)
{
    if (cache_len >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < new_len) new_cap = new_len;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* main sequence routine */
seq_t a(idx_t n)
{
    /* generate values until we have at least n terms */
    while (cache_len < n)
    {
        idx_t k = next_k;
        /* search for the next k satisfying the prime condition */
        while (true)
        {
            if (is_prime((idx_t)(8 * k + 1)) && is_prime((idx_t)(8 * k + 7)))
                break;
            ++k;
        }

        /* store the found k */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = (seq_t)k;
        ++cache_len;
        next_k = k + 1;
    }

    return cache[n - 1];
}
