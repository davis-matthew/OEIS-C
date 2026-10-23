/*
 * A028823  Numbers k such that k^2 + k + 17 is prime.
 * https://oeis.org/A028823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1522, u32 n <= 72, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k*k + k + 17 is prime (OEIS A028823) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    /* Cache of accepted k values. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* capacity of cache in elements   */
    static idx_t cache_len = 0;   /* number of values stored         */
    static idx_t next_k = 0;      /* next integer k to test          */

    /* Grow the cache until we have at least n entries. */
    while (cache_len < n) {
        /* Double the capacity when full. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE); /* out of memory */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test the next k. */
        seq_t k   = (seq_t)next_k;
        seq_t val = k * k + k + (seq_t)17;
        if (is_prime_big(val)) {
            cache[cache_len++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
