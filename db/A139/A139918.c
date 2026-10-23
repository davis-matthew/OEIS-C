/*
 * A139918  Primes of the form 8x^2+8xy+37y^2.
 * https://oeis.org/A139918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   admissible_residue, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139918:
 *    Primes of the form 8*x^2 + 8*x*y + 37*y^2.
 *    All such primes are congruent to one of
 *    {37, 53, 93, 197, 253, 277} (mod 280).
 *    We generate successive primes and keep the ones
 *    that satisfy this congruence.
 */

#include <stddef.h>  /* size_t */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/admissible_residue.h"

/* Return the n-th prime of the form 8*x^2+8*x*y+37*y^2 (OEIS offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    /* Static cache of admissible primes (as uint64_t for portable storage). */
    static uint64_t *primes = NULL;   /* array of prime values */
    static size_t    primes_cap = 0;   /* allocated capacity */
    static size_t    primes_len = 0;   /* current number of stored primes */
    static uint64_t  next_candidate = 2; /* next integer to search from */

    /* Ensure initial allocation. */
    if (primes == NULL) {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes)
            return (seq_t)0; /* allocation failure; value is irrelevant */
    }

    /* Generate primes until we have at least n admissible ones. */
    while (primes_len < (size_t)n) {
        uint64_t p = next_prime(next_candidate);
        next_candidate = p + 1;   /* next search starts just after p */

        if (!admissible_residue((int)(p % 280)))
            continue;            /* not of the required form */

        /* Grow the array if necessary. */
        if (primes_len == primes_cap) {
            size_t new_cap = primes_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) {
                /* Allocation failure – free and return zero. */
                free(primes);
                primes = NULL;
                primes_cap = 0;
                primes_len  = 0;
                return (seq_t)0;
            }
            primes = tmp;
            primes_cap = new_cap;
        }

        primes[primes_len++] = p;   /* store the admissible prime */
    }

    return (seq_t)primes[n - 1];
}
