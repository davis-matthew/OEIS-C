/*
 * A142198  Primes congruent to 39 mod 40.
 * https://oeis.org/A142198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 413, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that p ≡ 39 (mod 40) (OEIS A142198). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t size = 0;          /* number of cached terms */

    /* initialize the cache with the first term */
    if (size == 0) {
        capacity = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        cache[0] = (seq_t)79;   /* a(1) = 79 */
        size = 1;
    }

    /* generate terms until we have at least n terms */
    while (size < n) {
        /* start searching from the last cached prime */
        uint64_t p = (uint64_t)cache[size - 1];
        /* find the next prime that is ≡ 39 (mod 40) */
        do {
            p = next_prime(p);          /* returns the next prime > p */
        } while ((p % 40) != 39);

        /* reserve more space if necessary */
        if (size == capacity) {
            capacity = capacity ? capacity * 2 : 16;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
        }
        cache[size++] = (seq_t)p;
    }

    return cache[n - 1];
}
