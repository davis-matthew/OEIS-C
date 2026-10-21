/*
 * A215391  Primes congruent to {1, 2} mod 13.
 * https://oeis.org/A215391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n where p_n is the n-th prime congruent to 1 or 2 (mod
 *            13)
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, next_prime, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static cache for the primes of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_prime = 0;   /* last prime examined (inclusive) */

    /* first call: initialise with prime 2 if it qualifies */
    if (cache_len == 0) {
        if (2 % 13 == 1 || 2 % 13 == 2) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)2);
        }
        last_prime = 2;
    }

    /* extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        last_prime = next_prime(last_prime);          /* next prime > last_prime */
        if (last_prime % 13 == 1 || last_prime % 13 == 2) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)last_prime);
        }
    }

    return cache[n - 1];
}
