/*
 * A142446  Primes congruent to 38 mod 49.
 * https://oeis.org/A142446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 154, u32 n <= 66, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime congruent to 38 modulo 49 by testing
   successive candidates. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the nth prime with residue 38 modulo 49 (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *primes      = NULL;   /* array of primes */
    static size_t primes_len  = 0;      /* number of primes stored */
    static size_t primes_cap  = 0;      /* current capacity */
    static seq_t next_candidate = 38;   /* next number ≡ 38 (mod 49) */

    /* Grow the cache until it contains at least n primes. */
    while (primes_len < (size_t)n)
    {
        /* Advance until the next prime is found. */
        while (!is_prime_big(next_candidate))
            next_candidate += 49;

        /* Grow the array if necessary. */
        if (primes_len == primes_cap)
        {
            size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) abort();              /* allocation failure */
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Store the discovered prime. */
        primes[primes_len++] = next_candidate;

        /* Advance to the next candidate with the same residue. */
        next_candidate += 49;
    }

    /* Return the requested element (1‑based index). */
    return primes[n - 1];
}
