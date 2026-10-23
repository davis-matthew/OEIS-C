/*
 * A191061  Primes p that have Kronecker symbol (p|14) = -1.
 * https://oeis.org/A191061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   in_residue_set, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine returns the n‑th prime p with Kronecker symbol (p|14) = -1.
   Equivalently, p ≡ 11,17,29,31,33,37,41,43,47,51,53,55 (mod 56). */

/* Embedded search of successive primes in the specified residue classes,
   with a simple dynamic cache so that calls with arbitrary n are supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/in_residue_set.h"

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *cache = NULL;   /* array of primes */
    static idx_t count = 0;       /* number of cached primes */
    static idx_t cap = 0;         /* allocated capacity */
    static seq_t candidate = 3;   /* next odd number to test */

    /* Return cached result if already available. */
    if (n <= count)
        return cache[n - 1];

    /* Generate successive primes until we have n terms. */
    while (count < n) {
        if (is_prime_big(candidate)) {
            if (in_residue_set(candidate)) {
                /* Grow the cache if needed. */
                if (count == cap) {
                    idx_t new_cap = cap == 0 ? 16 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)  /* allocation failed – abort gracefully */
                        return 0;
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[count++] = candidate;
            }
        }
        candidate += 2;   /* skip even numbers */
    }

    return cache[n - 1];
}
