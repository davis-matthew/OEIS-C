/*
 * A173059  Nonnegative numbers k such that 2*k + 17 is prime.
 * https://oeis.org/A173059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonnegative integers k such that 2*k + 17 is prime. */
/* OEIS: A173059 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache holds computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_k     = 0;   /* next k to test */

    /* Initialise cache on first call. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory */
    }

    /* Expand cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t k = (seq_t)next_k;
        /* Compute 2*k+17 using a wider type to avoid overflow. */
        uint64_t p_big = (uint64_t)k * 2 + 17;
        seq_t p = (seq_t)p_big;

        /* Test primality of p. */
        if (is_prime_big(p)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache) return 0; /* out of memory */
                memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
                free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        next_k++;
    }

    /* Return the (n‑1)th element because OEIS offset is 1. */
    return cache[n - 1];
}
