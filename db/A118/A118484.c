/*
 * A118484  Primes p such that the nearest integer to p * gamma is prime,
 *          where gamma is Euler's constant.
 * https://oeis.org/A118484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that nearest integer to p * γ is prime. We
 *            use the rational approximation P/Q of γ with Q=10^10,
 *            P=5772156649 (i.e. γ ≈ 0.5772156649). For a prime p we compute q
 *            = round(p * γ) = (p*P + Q/2)/Q using only integer arithmetic
 * Range:     u8 n <= 13, u16 n <= 710, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"     /* provides bool is_prime(idx_t) */

/* Constants for the rational approximation of Euler's constant */
static const unsigned long long P = 5772156649ULL;   /* γ ≈ P / 10^10 */
static const unsigned long long Q = 10000000000ULL; /* Q = 10^10             */
static const unsigned long long HALF_Q = Q >> 1;    /* Q/2 for rounding */

/* Storage for the generated sequence values           */
static seq_t *terms      = NULL;   /* array of primes satisfying the condition */
static size_t terms_cap  = 0;      /* capacity of the array */
static size_t terms_cnt  = 0;      /* number of elements stored */

/* Current prime candidate to test for primality */
static idx_t prime_candidate = 1;  /* will be incremented to 2 on first use */

/* Grow the terms array when necessary */
static void grow_terms(void) {
    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    terms = new_ptr;
    terms_cap = new_cap;
}

/* Find the next prime ≥ prime_candidate + 1 and return it.
   prime_candidate is updated to this prime. */
static void find_next_prime(void) {
    do {
        ++prime_candidate;
        if (prime_candidate < 2) prime_candidate = 2;
    } while (!is_prime(prime_candidate));
}

/* Compute q = round(p * γ) using the rational approximation. */
static idx_t rounded_gamma_product(idx_t p) {
    unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)P;
    unsigned __int128 temp = prod + HALF_Q;
    return (idx_t)(temp / Q);
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety for offset < 1 */

    /* Generate terms until we have at least n of them */
    while (terms_cnt < n) {
        /* Find the next prime candidate */
        find_next_prime();

        /* Compute the nearest integer to p * γ */
        idx_t q = rounded_gamma_product(prime_candidate);

        /* Check if q is prime */
        if (is_prime(q)) {
            /* Store the prime p as a seq_t value */
            if (terms_cnt == terms_cap) grow_terms();
            terms[terms_cnt++] = (seq_t)prime_candidate;
        }
    }

    return terms[n - 1];
}
