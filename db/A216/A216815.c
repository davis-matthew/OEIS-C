/*
 * A216815  Primes congruent to 1 or 9 mod 20.
 * https://oeis.org/A216815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216815: primes congruent to 1 or 9 modulo 20 (offset 1) */
/* The function builds the sequence incrementally, caching results so
   subsequent calls require no recomputation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper function provided by the utilities: tests primality of an arbitrary-precision integer. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return zero as a placeholder. */
    if (n == 0) return (seq_t)0;

    /* Static cache holding the sequence values. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  count  = 0;

    /* Static candidate for generating the next odd integer. */
    static seq_t next = 3;

    /* Ensure the cache has at least one slot. */
    if (cap == 0) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
    }

    /* Fast return if we already have the requested term. */
    if (count >= (size_t)n) return cache[n - 1];

    /* Generate terms until the nth is reached. */
    while (count < (size_t)n) {
        /* Check if the current candidate is prime. */
        if (is_prime_big(next)) {
            /* Record primes that are 1 or 9 modulo 20. */
            if ((next % 20 == 1) || (next % 20 == 9)) {
                /* Grow the cache if necessary. */
                if (count == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;  /* allocation failure */
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[count++] = next;
            }
        }
        /* Advance to the next odd number. */
        next += 2;
    }

    return cache[n - 1];
}
