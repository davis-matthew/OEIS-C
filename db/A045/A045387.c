/*
 * A045387  Primes congruent to {2, 4} mod 7.
 * https://oeis.org/A045387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 2 or 4 modulo 7
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* prototype: bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    /* Static cache of the found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The next number to test for primality (start at 2) */
    static uint64_t candidate = 2;

    /* Allocate the cache on first entry */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n entries */
    while (cache_len < n) {
        /* Test candidate for primality */
        if (is_prime(candidate) &&
            (candidate % 7ull == 2 || candidate % 7ull == 4)) {

            /* Ensure cache capacity */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }

            cache[cache_len++] = (seq_t)candidate;
        }
        /* Move to the next number */
        candidate++;
    }

    /* idx_t is 1‑based here: a(1) is first prime in cache */
    return cache[n - 1];
}
