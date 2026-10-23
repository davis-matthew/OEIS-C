/*
 * A142163  Primes congruent to 7 mod 39.
 * https://oeis.org/A142163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 7 mod 39
 * Range:     u8 n <= 3, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

/* memoised cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Allocate or grow the cache array */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but guard against accidental zero */
        return 0;

    /* Ensure we have computed enough terms */
    if (cache_len < (size_t)n) {
        /* Maintain the last prime examined so far */
        static uint64_t last_prime = 0;
        grow_cache(n);

        while (cache_len < (size_t)n) {
            last_prime = next_prime(last_prime + 1);
            if (last_prime % 39 == 7) {
                cache[cache_len++] = (seq_t)last_prime;
            }
        }
    }
    return cache[n - 1];
}
