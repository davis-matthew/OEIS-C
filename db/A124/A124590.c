/*
 * A124590  Primes p such that q-p <= 6, where q is the next prime after p.
 * https://oeis.org/A124590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p whose next prime q satisfies q - p <= 6
 *            (equivalently, primes with a prime gap of ≤ 6). This
 *            implementation generates successive primes and records a prime
 *            when the next prime found is at most 6 greater
 * Range:     u8 n <= 47, u16 n <= 3097, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* static storage for the list of qualifying primes */
static seq_t *qual = NULL;
static size_t qual_len = 0, qual_cap = 0;

/* static storage for all primes found so far */
static uint64_t *prime_list = NULL;
static size_t prime_len = 0, prime_cap = 0;

/* current candidate for primality testing (initially 2) */
static uint64_t next_candidate = 2;

seq_t a(idx_t n) {
    /* 1‑based index: produce the nth qualifying prime */
    size_t target = (size_t)n;

    /* Expand the list until we have enough terms */
    while (qual_len < target) {
        uint64_t cand = next_candidate;
        bool is_prime = true;

        /* primality test by trial division using previously found primes */
        uint64_t limit = (uint64_t)sqrt((double)cand);
        for (size_t i = 0; i < prime_len && prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Ensure capacity for the primes array */
            if (prime_len == prime_cap) {
                prime_cap = (prime_cap ? prime_cap * 2 : 8);
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;

            /* Check if the previous prime qualifies */
            if (prime_len >= 2) {
                uint64_t prev = prime_list[prime_len - 2];
                if (cand - prev <= 6U) {
                    /* Ensure capacity for the qualifying primes array */
                    if (qual_len == qual_cap) {
                        qual_cap = (qual_cap ? qual_cap * 2 : 8);
                        qual = (seq_t*)realloc(qual,
                                                qual_cap * sizeof(seq_t));
                    }
                    qual[qual_len++] = (seq_t)prev;
                }
            }
        }

        /* Advance to the next odd candidate */
        if (cand == 2) {
            next_candidate = 3;
        } else {
            next_candidate += 2;
        }
    }

    return qual[target - 1];
}
