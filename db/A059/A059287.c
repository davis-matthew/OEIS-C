/*
 * A059287  Primes p such that x^16 = 2 has no solution mod p, but x^8 = 2 has
 *          a solution mod p.
 * https://oeis.org/A059287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 107, u32 n <= 150, u64 n <= 150, u128 n <= 150,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes p such that
   x^16 = 2 has no solution modulo p but
   x^8  = 2 has a solution modulo p.
   The algorithm sieves for primes, tests the
   congruence conditions using modular exponentiation,
   and stores all qualifying primes in a static
   array.  a(n) returns the (n‑1)th element of that array. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   // mod_pow(seq_t base, uint64_t exp, seq_t mod)

seq_t a(idx_t n) {
    /* static state */
    static bool built = false;
    static uint64_t *good = NULL;      /* array of qualifying primes */
    static size_t good_cnt = 0;
    static size_t good_cap = 0;

    if (!built) {
        /* Limits chosen to generate >100 000 qualifying primes
           (≈48 M sieve limit gives ~106 k good primes). */
        const uint64_t LIMIT = 48000000ULL;
        /* Boolean array (char) for odd numbers only: index i represents 2*i+1 */
        size_t half = (LIMIT >> 1) + 1;
        char *is_comp = (char *)calloc(half, 1);   /* 0 = prime, 1 = composite */
        if (!is_comp) return 0;  /* allocation failure */

        /* Sieve of Eratosthenes (odd numbers only) */
        uint64_t i, j;
        for (i = 3; i * i <= LIMIT; i += 2) {
            if (!is_comp[i >> 1]) {
                uint64_t step = i << 1;
                for (j = i * i; j <= LIMIT; j += step)
                    is_comp[j >> 1] = 1;
            }
        }

        /* Prepare dynamic array for good primes */
        good_cap = 1024;
        good = (uint64_t *)malloc(good_cap * sizeof(uint64_t));
        if (!good) { free(is_comp); return 0; }

        /* Iterate over primes, test conditions */
        const seq_t one = 1;
        for (i = 3; i <= LIMIT; i += 2) {
            if (is_comp[i >> 1]) continue;   /* not prime */
            if (i % 16 != 1) continue;       /* need p ≡ 1 (mod 16) */

            seq_t p = (seq_t)i;
            uint64_t exp8  = (i - 1ULL) / 8ULL;
            uint64_t exp16 = (i - 1ULL) / 16ULL;

            seq_t r8  = mod_pow(2, exp8,  p);
            if (r8 != one) continue;         /* x^8 = 2 has no solution */

            seq_t r16 = mod_pow(2, exp16, p);
            if (r16 == one) continue;        /* x^16 = 2 would have a solution */

            /* Store qualifying prime */
            if (good_cnt == good_cap) {
                good_cap = good_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(good, good_cap * sizeof(uint64_t));
                if (!tmp) { free(is_comp); free(good); return 0; }
                good = tmp;
            }
            good[good_cnt++] = i;
        }

        free(is_comp);
        built = true;
    }

    if (n == 0 || (size_t)n > good_cnt) return 0;   /* out of bounds */
    return (seq_t)good[n - 1];
}
