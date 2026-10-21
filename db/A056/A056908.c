/*
 * A056908  Numbers k such that 36*k^2 + 36*k + 13 is prime.
 * https://oeis.org/A056908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 15, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the k such that 36*k^2 + 36*k + 13 is prime (sequence A056908). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Static state to cache previously found terms and the next candidate k. */
    static seq_t *terms = NULL;          /* array of k-values */
    static idx_t term_count = 0;         /* number of terms stored */
    static idx_t capacity = 0;           /* current array capacity */
    static idx_t next_candidate = 0;     /* next k to test */

    /* Initialise storage on first call. */
    if (!terms) {
        capacity = 16;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Generate terms until we have at least n terms. */
    while (term_count < n) {
        seq_t k = (seq_t)next_candidate;
        seq_t p = (seq_t)36 * k * k + (seq_t)36 * k + (seq_t)13;

        /* Test primality of p. */
        if (is_prime_big(p)) {
            /* Resize if necessary. */
            if (term_count == capacity) {
                capacity *= 2;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[term_count++] = k;   /* store the k that yields a prime */
        }

        ++next_candidate;   /* move to next k */
    }

    return terms[n - 1]; /* offset 1: a(1) is the first term listed */
}
