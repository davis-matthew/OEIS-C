/*
 * A098976  Upper prime of a difference of 22 between consecutive primes.
 * https://oeis.org/A098976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 145, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098976: Upper prime of a difference of 22 between consecutive primes
 *  a(n) = A061779(n) + 22
 *  The function generates the sequence on demand, memoised for future
 *  calls.  It scans the primes using next_prime and records each time a
 *  gap of 22 occurs.  The implementation works for any seq_t and idx_t
 *  supplied by the build.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static seq_t *terms = NULL;      /* memoised sequence values   */
static size_t terms_len = 0;      /* number of terms stored     */
static size_t terms_cap = 0;      /* capacity of the array      */
static uint64_t last_prime = 0;   /* last prime processed       */
static bool initialized = false; /* initialisation flag        */

/* Ensure that at least `n` terms have been generated.
 * Uses the efficient next_prime function to step through consecutive
 * primes and records every upper prime of a 22‑gap. */
static void ensure_terms(idx_t n)
{
    if (!initialized) {
        initialized = true;
        last_prime = 0;                      /* start from before first prime */
        terms_cap = 64;                      /* initial capacity              */
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Grow array if necessary */
    if (terms_len < n && terms_len == terms_cap) {
        terms_cap *= 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Generate primes until we have `n` terms */
    while (terms_len < n) {
        uint64_t prev = last_prime;
        uint64_t curr = next_prime(prev + 1); /* next prime after `prev` */

        if (curr - prev == 22U) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)curr;   /* record upper prime of 22-gap */
        }
        last_prime = curr;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[n - 1];
}
