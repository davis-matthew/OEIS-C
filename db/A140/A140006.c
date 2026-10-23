/*
 * A140006  Primes of the form 15x^2+88y^2.
 * https://oeis.org/A140006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 204, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 15*x^2 + 88*y^2 (search via residue filter) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Helper: does p mod 1320 belong to the known residue classes? */
static bool matches_residue(idx_t p)
{
    static const idx_t residues[10] = {
        103, 223, 247, 367, 463,
        487, 727, 823, 1087, 1303
    };
    idx_t r = p % 1320;
    for (int i = 0; i < 10; ++i) {
        if (r == residues[i]) return true;
    }
    return false;
}

/* Main routine: returns the n-th prime of the form 15*x^2 + 88*y^2
 *  a(1) = 103.  The function keeps a cache of all found primes so that
 *  subsequent calls (in any order) are fast. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached primes */
    static size_t cap = 0;           /* capacity of cache array   */
    static size_t cnt = 0;           /* number of cached primes   */
    static idx_t next_candidate = 3; /* next odd number to test   */

    /* Grow cache as needed */
    if (cnt == 0) {
        cap = 64;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
    }

    /* Generate primes until we have at least n of them */
    while (cnt < n) {
        if (is_prime(next_candidate) && matches_residue(next_candidate)) {
            /* Store prime into cache */
            if (cnt == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[cnt++] = (seq_t)next_candidate;
        }
        /* Advance to the next odd integer */
        next_candidate += 2;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
