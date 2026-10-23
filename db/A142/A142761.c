/*
 * A142761  Primes congruent to 34 mod 59.
 * https://oeis.org/A142761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime congruent to 34 modulo 59.
   The sequence is defined as all primes p such that p % 59 == 34,
   with the offset a(1) = 211.  The function builds the sequence
   lazily, expanding a static array as required.  It uses the
   is_prime(idx_t) utility to test primality. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"      /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *primes = NULL;
    static size_t  len   = 0;
    static size_t  cap   = 0;

    /* Initialise cache on first call */
    if (primes == NULL) {
        cap   = 64;
        primes = (seq_t *)malloc(cap * sizeof(*primes));
    }

    /* Generate successive primes until we have the nth one */
    for (idx_t need = (idx_t)(len + 1); need <= n; ++need) {
        idx_t  prev   = (len > 0) ? (idx_t)primes[len - 1] : 0;
        idx_t  cand   = 34;                 /* start of residue class */

        /* Skip candidates <= previous prime */
        while (cand <= prev)
            cand += 59;

        /* Find the next prime in this residue class */
        while (!is_prime(cand))
            cand += 59;

        /* Grow the cache if necessary */
        if (len >= cap) {
            cap  *= 2;
            primes = (seq_t *)realloc(primes, cap * sizeof(*primes));
        }

        primes[len++] = (seq_t)cand;
    }

    return primes[n - 1];
}
