/*
 * A139643  Primes of the form x^2+Ny^2, with N=102.
 * https://oeis.org/A139643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime of the form x^2 + 102*y^2.
   Equivalently, it returns the n‑th prime congruent to
   {1, 25, 49, 55, 103, 121, 127, 145, 151, 169, 217, 223, 247, 271, 319, 361}
   (mod 408).  The result is found by a simple sieve–like search
   that tests only candidates in those residues and checks primality
   with the provided is_prime routine.  The function keeps a static
   cache so that calls in any order remain fast. */

#include "../utils/is_prime.h"
#include <stdbool.h>
#include <stdlib.h>

/* 16 residues modulo 408 that produce the desired primes */
static const unsigned short residues[16] = {
    1, 25, 49, 55, 103, 121, 127, 145,
    151, 169, 217, 223, 247, 271, 319, 361
};

/* Return the n‑th prime of this form (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; guard against 0 */

    static bool mask_initialised = false;
    static bool allowed[408] = {0};

    static seq_t *cache     = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static idx_t candidate  = 2;   /* next odd number to test */

    /* Initialise the residue mask once. */
    if (!mask_initialised) {
        for (int i = 0; i < 408; ++i) allowed[i] = false;
        for (int i = 0; i < 16; ++i) allowed[residues[i]] = true;
        mask_initialised = true;
    }

    /* Generate primes until we have at least n entries. */
    while (cache_size < (size_t)n) {
        /* Ensure candidate is odd. */
        if ((candidate & 1ULL) == 0) candidate++;

        /* Test only allowed residues. */
        if (allowed[(int)(candidate % 408)]) {
            if (is_prime(candidate)) {   /* primality test from utils */
                if (cache_size >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* allocation failure */
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)candidate;
            }
        }
        candidate += 2;   /* next odd number */
    }

    return cache[n - 1];
}
