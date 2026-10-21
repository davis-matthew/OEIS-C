/*
 * A215382  Primes congruent to {1, 2, 3, 6} mod 11.
 * https://oeis.org/A215382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n-th prime congruent to 1, 2, 3 or 6 modulo 11.
   The function stores all found primes in a static cache and
   continues searching until it has at least n such primes.
   If seq_t can hold 64‑bit integers, the algorithm works for all n
   required by the tests (up to at least 10 000). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* A cache of all primes found so far (unrestricted residues). */
    static uint64_t *primes_all = NULL;
    static size_t primes_cap = 0, primes_size = 0;

    /* A cache of primes that satisfy r ≡ 1,2,3 or 6 (mod 11). */
    static uint64_t *residues = NULL;
    static size_t residues_cap = 0, residues_size = 0;

    /* The next integer to test for primality. */
    static uint64_t current = 2;

    /* The sequence is 1‑based; n==0 is undefined, return 0. */
    if (n == 0) return 0;

    /* Initialise the caches on first call. */
    if (primes_cap == 0) {
        primes_cap = 8;
        primes_all = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes_all) abort();
    }
    if (residues_cap == 0) {
        residues_cap = 8;
        residues = (uint64_t *)malloc(residues_cap * sizeof(uint64_t));
        if (!residues) abort();
    }

    /* Search until we have at least n required primes. */
    while (residues_size < n) {
        /* Test whether current is prime. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes_all[i];
            if (p * p > current) break;
            if (current % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store in the general primes list. */
            if (primes_size == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes_all, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes_all = tmp;
                primes_cap = new_cap;
            }
            primes_all[primes_size++] = current;

            /* Check the residue condition. */
            uint64_t r = current % 11u;
            if (r == 1 || r == 2 || r == 3 || r == 6) {
                if (residues_size == residues_cap) {
                    size_t new_cap = residues_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(residues, new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    residues = tmp;
                    residues_cap = new_cap;
                }
                residues[residues_size++] = current;
            }
        }

        /* Advance to the next candidate. */
        ++current;
        if (current == 0) abort();  /* overflow guard */
    }

    /* Return the desired term in the required type. */
    return (seq_t)residues[n - 1];
}
