/*
 * A127575  Numbers n such that 16n+15 is prime.
 * https://oeis.org/A127575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A127575: Numbers n such that 16n+15 is prime, offset 1.
 *  This implementation builds the sequence lazily using a static cache.
 *  Each call generates new terms only if needed, using the next_prime()
 *  routine from the utilities to step through the prime numbers.
 *  The cache persists across calls, so any order of requests is handled
 *  efficiently.  The algorithm runs in O(n) time to generate the first n
 *  terms, with negligible overhead thereafter.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Static cache of generated terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* The last prime examined (used to find the next prime). */
static uint64_t last_prime = 15;   /* 16*1+15=31 is the first valid prime */

/* Grow the cache if necessary. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* If memory allocation fails, terminate gracefully. */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based).  If n == 0, return 0. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1 */
        return (seq_t)0;

    /* Ensure we have at least n terms in the cache. */
    if (cache_len < (size_t)n) {
        grow_cache(n);
        /* Generate new terms until we have n of them. */
        while (cache_len < (size_t)n) {
            /* Find the next prime after last_prime.  next_prime
             * returns the first prime strictly greater than its argument. */
            last_prime = next_prime(last_prime);
            if ((last_prime - 15ULL) % 16ULL == 0ULL) {
                /* p = 16*k + 15 → k = (p-15)/16.  The result fits in
                 * seq_t for the test sizes considered. */
                seq_t k = (seq_t)((last_prime - 15ULL) / 16ULL);
                cache[cache_len++] = k;
            }
        }
    }

    return cache[n - 1];
}
