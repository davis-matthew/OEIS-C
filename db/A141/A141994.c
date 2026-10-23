/*
 * A141994  Primes congruent to 18 mod 29.
 * https://oeis.org/A141994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 245, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A141994: primes congruent to 18 (mod 29).
   Method: iterate through numbers of the form 29*m+18 and test each for
   primality with is_prime(idx_t).  Store found primes in a static cache
   so later calls are O(1).  The first prime in the sequence is 47. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset starts at 1 */

    /* static cache for the primes already found */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Extend the cache until it contains at least n primes. */
    if (cache_size < n) {
        /* initialise cache if needed */
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }

        /* start candidate value: first prime is 47; otherwise take last cached
           prime and add 29 to get the next number that is 18 (mod 29). */
        idx_t candidate;
        if (cache_size == 0)
            candidate = 47;
        else
            candidate = ((idx_t)cache[cache_size - 1]) + 29;

        /* search for further primes */
        while (cache_size < n) {
            if (is_prime(candidate)) {
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = (seq_t)candidate;
            }
            candidate += 29;
        }
    }

    return cache[n - 1];
}
