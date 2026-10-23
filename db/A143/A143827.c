/*
 * A143827  Numbers k such that 8*k^2 - 1 is prime.
 * https://oeis.org/A143827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that 8*k^2 - 1 is prime
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 37, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Return the n-th k with 8*k^2 - 1 prime.  Indices start at 1. */
seq_t a(idx_t n)
{
    /* Static cache of found k's. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    /* Next integer to test. */
    static idx_t next_k = 1;

    /* Grow cache if necessary. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend cache until we have n terms. */
    while (cache_cnt < n) {
        /* Compute 8*next_k^2 - 1.  Use seq_t to avoid overflow. */
        seq_t k = (seq_t)next_k;
        seq_t val = 8 * k * k - 1;

        /* Test primality of val. */
        if (is_prime_big(val)) {
            /* Ensure enough space. */
            if (cache_cnt == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
