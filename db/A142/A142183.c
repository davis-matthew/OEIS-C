/*
 * A142183  Primes congruent to 1 mod 40.
 * https://oeis.org/A142183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 378, u32 n <= 379, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes ≡ 1 (mod 40), offset 1.  Computed by incremental search
   of candidates. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of primes congruent to 1 modulo 40. */
    static seq_t *primes = NULL;
    static size_t  cap   = 0;
    static idx_t   len   = 0;
    static seq_t last = 0;   /* last prime stored */

    /* Allocate initial storage if needed. */
    if (primes == NULL) {
        cap = 256;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (primes == NULL) {
            /* Allocation failed – cannot continue. */
            exit(EXIT_FAILURE);
        }
    }

    /* Generate further primes until we reach the requested index. */
    while (len < n) {
        seq_t candidate;

        if (len == 0) {
            /* First prime in the sequence. */
            candidate = (seq_t)41;
        } else {
            /* Next candidate must be 40 higher than the last found prime. */
            candidate = last;
            candidate += (seq_t)40;
            while (!is_prime_big(candidate)) {
                candidate += (seq_t)40;
            }
        }

        /* Grow cache if necessary. */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* Re‑allocation failed – clean up and terminate. */
                free(primes);
                exit(EXIT_FAILURE);
            }
            primes = tmp;
            cap = new_cap;
        }

        primes[len++] = candidate;
        last = candidate;
    }

    return primes[n - 1];
}
