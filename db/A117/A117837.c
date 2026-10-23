/*
 * A117837  Smaller of two consecutive prime numbers with the same digital
 *          product.
 * https://oeis.org/A117837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A117837 – smaller of two consecutive prime numbers with the same digital product.
   The algorithm searches consecutive primes and records those whose digital products
   (product of decimal digits) coincide.  Results are memoised so that out–of–order
   requests are answered efficiently. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../utils/digit_product.h"
#include "../utils/next_prime.h"

/* Return the n-th (1‑based) term of the sequence.  */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State while generating primes. */
    static uint64_t prev_prime = 0;
    static uint64_t curr_prime = 0;
    static bool     initialized = false;

    /* Initialise on first call. */
    if (!initialized) {
        prev_prime = 2ULL;
        curr_prime = next_prime(prev_prime);
        cache_len   = 0;
        cache_cap   = 0;
        cache       = NULL;
        initialized = true;
    }

    /* Ensure we have at least n entries in the cache. */
    while (cache_len < (size_t)n) {
        seq_t prod_prev = digit_product((seq_t)prev_prime);
        seq_t prod_curr = digit_product((seq_t)curr_prime);

        if (prod_prev == prod_curr) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_len + 1)
                    new_cap = cache_len + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache       = tmp;
                cache_cap   = new_cap;
            }
            cache[cache_len++] = (seq_t)prev_prime;
        }

        /* Advance to the next consecutive primes. */
        prev_prime = curr_prime;
        curr_prime = next_prime(prev_prime);
    }

    return cache[(size_t)n - 1];
}
