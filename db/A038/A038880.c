/*
 * A038880  Primes p such that 10 is not a square mod p.
 * https://oeis.org/A038880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038880: primes p such that 10 is not a square mod p
   (equivalently, p ≡ 7, 11, 17, 19, 21, 23, 29, or 33 (mod 40)).
   The function generates primes incrementally, memoises the
   qualifying ones, and returns the nth term (1‑based). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* --- memoised cache of qualifying primes (as 64‑bit values) --- */
    static uint64_t *terms   = NULL;  /* array of stored primes   */
    static size_t   terms_cap = 0;    /* current capacity of array */
    static size_t   terms_sz  = 0;    /* number of primes stored  */

    /* next prime to inspect; starts at the first odd prime */
    static uint64_t next_to_test = 3;

    /* sanity: n must be ≥ 1; calls with n == 0 are not expected but
       handled safely by returning 0. */
    if (n == 0) return (seq_t)0;

    /* fill the cache until it contains at least n terms */
    while (terms_sz < n) {
        uint64_t p = next_to_test;
        uint32_t r = (uint32_t)(p % 40U);

        /* test the residue condition */
        if (r == 7U || r == 11U || r == 17U || r == 19U ||
            r == 21U || r == 23U || r == 29U || r == 33U) {

            /* grow the array if necessary */
            if (terms_sz == terms_cap) {
                size_t new_cap = (terms_cap == 0) ? 8 : (terms_cap << 1);
                uint64_t *tmp =
                    (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_sz++] = p;
        }

        /* advance to the next prime candidate */
        next_to_test = next_prime(p + 1);
    }

    /* convert the stored 64‑bit value to the requested seq_t type */
    return (seq_t)terms[n - 1];
}
