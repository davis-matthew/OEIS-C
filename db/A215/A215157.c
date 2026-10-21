/*
 * A215157  Primes congruent to {2, 3, 5, 7} mod 19.
 * https://oeis.org/A215157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes p with p ≡ 2, 3, 5 or 7 (mod 19). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Memoised prime and selected‑prime tables */
    static uint64_t *primes      = NULL;
    static size_t   primes_size  = 0;
    static size_t   primes_cap   = 0;
    static uint64_t *sel         = NULL;   /* only uint64_t values stored */
    static size_t   sel_size     = 0;
    static size_t   sel_cap      = 0;
    static uint64_t current      = 2;      /* next number to test */

    /* If we already have the requested term, return it */
    if (sel_size >= n)
        return (seq_t)sel[n - 1];

    /* Grow the tables to a reasonable initial capacity */
    if (primes_cap == 0) {
        primes_cap = 128;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }
    if (sel_cap == 0) {
        sel_cap = 16;
        sel = (uint64_t*)malloc(sel_cap * sizeof(uint64_t));
    }

    /* Generate primes until we have enough selected terms */
    while (sel_size < n) {
        /* Primality test by trial division using already found primes */
        bool is_prime = true;
        uint64_t max_div = int_sqrt(current);
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if (p > max_div) break;
            if (current % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime in the primes table */
            if (primes_size == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_size++] = current;

            /* Check the residue condition and store if it matches */
            uint64_t r = current % 19;
            if (r == 2 || r == 3 || r == 5 || r == 7) {
                if (sel_size == sel_cap) {
                    sel_cap *= 2;
                    sel = (uint64_t*)realloc(sel, sel_cap * sizeof(uint64_t));
                }
                sel[sel_size++] = current;
            }
        }

        ++current;
    }

    return (seq_t)sel[n - 1];
}
