/*
 * A142124  Primes congruent to 15 mod 37.
 * https://oeis.org/A142124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n-th prime congruent to 15 (mod 37) by sequential
    prime searches using is_prime_small.  */

/* Include utilities */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    /* Static cache of found terms and sieve state */
    static seq_t *cache = NULL;         /* array of primes found */
    static size_t cache_cap = 0;        /* capacity of cache */
    static idx_t cache_count = 0;       /* number of cached primes */
    static seq_t next_candidate = (seq_t)15; /* next number congruent 15 mod 37 */
    static bool initialized = false;

    /* First time: allocate initial cache */
    if (!initialized) {
        initialized = true;
        cache_cap = 128; /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Grow cache if necessary */
    if (cache_count >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Find enough primes to answer the request */
    while (cache_count < (size_t)n) {
        /* Advance to the next number ≡15 (mod 37) that is prime.
           The loop stops when is_prime_small reports a prime. */
        do {
            next_candidate += (seq_t)37;
        } while (!is_prime_small(next_candidate));

        /* Store the found prime */
        if (cache_count >= cache_cap) {           /* double size if needed */
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_count++] = next_candidate;
    }

    return cache[n - 1];
}
