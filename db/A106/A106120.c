/*
 * A106120  Primes with maximal digit > 3.
 * https://oeis.org/A106120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   has_digit_gt3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th prime whose decimal representation contains
   a digit greater than 3.  The function keeps a static sieve of primes up
   to the current candidate and a growing list of qualifying terms.  The
   implementation is O(N) where N is the number of terms requested. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_digit_gt3.h"

/* ------------------------------------------------------------------ */
/*          static state (different from the old one)                  */
/* ------------------------------------------------------------------ */
static uint64_t candidate = 2;          /* next number to test          */

/* primes storage – dynamic array */
static uint64_t *primes   = NULL;
static size_t   primes_cap  = 0;
static size_t   primes_size = 0;

/* terms storage – dynamic array */
static seq_t *terms   = NULL;
static size_t   terms_cap  = 0;
static size_t   terms_size = 0;

/* ------------------------------------------------------------------ */
/*  helper: ensure capacity for the primes array                      */
/* ------------------------------------------------------------------ */
static void ensure_primes_cap(size_t min_cap) {
    if (primes_cap < min_cap) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes       = tmp;
        primes_cap   = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  helper: ensure capacity for the terms array                       */
/* ------------------------------------------------------------------ */
static void ensure_terms_cap(size_t min_cap) {
    if (terms_cap < min_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 64;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms      = tmp;
        terms_cap  = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  primality test using the current list of primes (trial division) */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t m) {
    if (m < 2)         return false;
    if (m == 2)        return true;
    if ((m & 1ULL) == 0) return false;

    for (size_t i = 0; i < primes_size; ++i) {
        uint64_t p = primes[i];
        if (p > m / p) break;          /* reached sqrt(m) */
        if (m % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  check whether a decimal number has a digit greater than 3         */

/* ------------------------------------------------------------------ */
/*  main function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* offset is 1 */

    /* initialise static vectors on first use */
    if (primes == NULL) {
        ensure_primes_cap(64);
        primes[0] = 2;
        primes_size = 1;
    }
    if (terms == NULL) {
        ensure_terms_cap(64);
    }

    /* generate terms until we have at least n of them */
    while (terms_size < (size_t)n) {
        /* test the candidate */
        if (is_prime_uint64(candidate)) {
            /* store the prime */
            ensure_primes_cap(primes_size + 1);
            primes[primes_size++] = candidate;

            /* store if it satisfies the digit condition */
            if (has_digit_gt3(candidate)) {
                ensure_terms_cap(terms_size + 1);
                terms[terms_size++] = (seq_t)candidate;
            }
        }

        /* advance to the next candidate (after 2 only odd numbers) */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }

    return terms[(size_t)(n - 1)];
}
