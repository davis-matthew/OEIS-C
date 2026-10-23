/*
 * A126330  Primes of the form 4p+3 where p is a prime.
 * https://oeis.org/A126330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 521, u32 n <= 838, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth prime of the form 4p+3 with p prime. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* static cache, filled on demand */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored */
static idx_t cache_cap  = 0;   /* allocated capacity */
static uint64_t last_checked_prime = 3;   /* prime from which we start searching */

static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* allocation failed – abort safely */
        exit(EXIT_FAILURE);
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1 – value for index 0 irrelevant */

    /* Ensure we have at least n entries in the cache */
    if (cache_size < n) {
        while (cache_size < n) {
            /* find the next prime q of the form 4k+3 */
            do {
                last_checked_prime = next_prime(last_checked_prime);
            } while (last_checked_prime % 4u != 3u);

            /* candidate prime q = last_checked_prime */
            uint64_t p = (last_checked_prime - 3u) / 4u;  /* p = (q-3)/4 */

            /* check if p is prime */
            bool p_is_prime = is_prime_big((seq_t)p);

            if (p_is_prime) {
                /* store q in cache */
                if (cache_size == cache_cap)
                    grow_cache();
                cache[cache_size++] = (seq_t)last_checked_prime;
            }
        }
    }

    /* return the requested element (offset 1) */
    return cache[n - 1];
}
