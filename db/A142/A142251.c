/*
 * A142251  Primes congruent to 2 mod 43.
 * https://oeis.org/A142251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 2 (mod 43)
 * Range:     u8 n <= 2, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute the nth prime congruent to 2 modulo 43. */
seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *primes = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;
    static idx_t next   = 2;           /* next number to test */

    /* Ensure we have at least n primes cached. */
    while (size < (size_t)n) {
        /* Find the next number ≡ 2 (mod 43) */
        while (next % 43 != 2) next++;

        if (is_prime(next)) {
            /* Grow cache if necessary */
            if (size + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failed – abort */
                    abort();
                }
                primes = tmp;
                cap    = new_cap;
            }
            primes[size++] = (seq_t)next;   /* store prime as seq_t */
        }
        next++;      /* move to next candidate */
    }

    return primes[n - 1];
}
