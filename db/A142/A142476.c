/*
 * A142476  Primes congruent to 1 mod 51.
 * https://oeis.org/A142476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142476: primes congruent to 1 (mod 51)
   a(n) = the nth prime p such that p ≡ 1 (mod 51), starting with a(1)=103.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the nth prime of the form 51k+1 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* Cached sequence values. */
    static seq_t *cache = NULL; /* array holding the primes found so far */
    static size_t size = 0;     /* number of stored primes */
    static size_t capacity = 0; /* allocated array size */
    static idx_t next = 2;      /* next integer to test for primality */

    /* Allocate storage on first call. */
    if (cache == NULL) {
        capacity = 32;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
    }

    /* Extend the cache until it contains at least n elements. */
    while (size < (size_t)n) {
        idx_t p = next;
        /* Find the next prime. */
        while (!is_prime(p))
            ++p;
        next = p + 1; /* next search will start after this prime */

        /* Keep only primes that are 1 (mod 51). */
        if ((p % 51) != 1)
            continue;

        /* Grow the cache if necessary. */
        if (size + 1 > capacity) {
            capacity *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            cache = tmp;
        }

        cache[size++] = (seq_t)p;
    }

    return cache[n - 1];
}
