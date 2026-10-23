/*
 * A142088  Primes congruent to 18 mod 35.
 * https://oeis.org/A142088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 273, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 18 (mod 35).
   a(n) = the nth prime p with p ≡ 18 (mod 35), offset = 1. */
/* This implementation finds successive primes in that residue class
   on demand, caching the results so that calls may arrive in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* offset 1: if n == 0 return 0 as a sentinel */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of previously found primes */
    static seq_t *cache    = NULL;   /* array holding the primes */
    static idx_t  cached   = 0;      /* number of primes stored */
    static idx_t candidate = 18;     /* next number to test, ≡18 (mod 35) */

    /* Grow the cache if we need more terms */
    if (cached < n) {
        /* Ensure capacity, double each time */
        size_t new_cap = (cached == 0) ? 16 : cached * 2;
        if (new_cap < n) new_cap = n;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
        cache = tmp;

        /* Find primes until we have at least n terms */
        while (cached < n) {
            candidate += 35;                         /* candidate ≡ 18 (mod 35) */
            if (is_prime(candidate))                   /* test primality */
                cache[cached++] = (seq_t)candidate;   /* store it */
        }
    }

    return cache[n - 1];    /* return the requested term (1‑based index) */
}
