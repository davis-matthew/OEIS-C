/*
 * A045387  Primes congruent to {2, 4} mod 7.
 * https://oeis.org/A045387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 2 or 4 modulo 7
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 893 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

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
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate primes until we have at least n entries */
    while (cache_len < n) {
        /* Test candidate for primality */
        if (is_prime(candidate) &&
            (candidate % 7ull == 2 || candidate % 7ull == 4)) {

            /* Ensure cache capacity */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }

            cache[cache_len++] = (seq_t)candidate;
        }
        /* Move to the next number */
        candidate++;
    }

    /* idx_t is 1‑based here: a(1) is first prime in cache */
    return cache[n - 1];
}
