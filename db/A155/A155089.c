/*
 * A155089  Primes p such that the largest digit of the concatenation of p and
 *          the p-th prime is either 1 or 9.
 * https://oeis.org/A155089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, max_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A155089: primes p such that the largest digit of the
   decimal concatenation of p and the p‑th prime is either 1 or 9.
   No closed formula is known; we generate primes via a sieve and test
   each candidate. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/max_digit_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static uint64_t sieve_limit = 0;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    static uint64_t *qualifiers = NULL;
    static size_t qual_cnt = 0;
    static size_t qual_cap = 0;

    /* ---------- First initialisation ---------- */
    if (primes == NULL) {
        sieve_limit = 1000000ULL;                 /* initial limit */
        build_primes_upto(sieve_limit, &primes, &prime_cnt);

        qual_cap = prime_cnt / 2 + 10;            /* rough estimate */
        qualifiers = (uint64_t*)malloc(qual_cap * sizeof(uint64_t));
        qual_cnt = 0;

        /* Find qualifying primes in the initial range */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > prime_cnt) break;             /* p‑th prime doesn't exist */
            uint64_t q = primes[p - 1];

            uint64_t md = max_digit_uint64(p);
            uint64_t d2 = max_digit_uint64(q);
            if (d2 > md) md = d2;

            if (md == 1 || md == 9) {
                if (qual_cnt == qual_cap) {
                    qual_cap <<= 1;
                    qualifiers = (uint64_t*)realloc(qualifiers,
                                                     qual_cap * sizeof(uint64_t));
                }
                qualifiers[qual_cnt++] = p;
            }
        }
    }

    /* ---------- Expand the sieve until we have enough terms ---------- */
    while (qual_cnt < n) {
        /* Double the sieve limit to grow the candidate set */
        sieve_limit <<= 1;
        free(primes);
        build_primes_upto(sieve_limit, &primes, &prime_cnt);

        /* Recompute qualifying primes from scratch for the new limit */
        free(qualifiers);
        qual_cap = prime_cnt / 2 + 10;            /* new estimate */
        qualifiers = (uint64_t*)malloc(qual_cap * sizeof(uint64_t));
        qual_cnt = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > prime_cnt) break;
            uint64_t q = primes[p - 1];

            uint64_t md = max_digit_uint64(p);
            uint64_t d2 = max_digit_uint64(q);
            if (d2 > md) md = d2;

            if (md == 1 || md == 9) {
                if (qual_cnt == qual_cap) {
                    qual_cap <<= 1;
                    qualifiers = (uint64_t*)realloc(qualifiers,
                                                     qual_cap * sizeof(uint64_t));
                }
                qualifiers[qual_cnt++] = p;
            }
        }
    }

    return (seq_t)qualifiers[n - 1];
}
