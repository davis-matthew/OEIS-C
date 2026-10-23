/*
 * A045469  Primes congruent to {1, 4} mod 7.
 * https://oeis.org/A045469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the n-th prime congruent to 1 or 4 (mod 7), offset 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_size = 0;
    static size_t primes_cap = 0;
    static seq_t last_checked = 0;

    /* initialize the first element */
    if (primes_size == 0) {
        primes_cap = 16;
        primes = (seq_t*)malloc(primes_cap * sizeof(*primes));
        primes[0] = (seq_t)11;
        primes_size = 1;
        last_checked = (seq_t)11;
    }

    /* if already computed, return directly */
    if ((size_t)n <= primes_size) {
        return primes[(size_t)n - 1];
    }

    /* otherwise generate until we have at least n primes */
    while (primes_size < (size_t)n) {
        seq_t cand = last_checked + (seq_t)2;   /* next odd candidate */
        for (;;) {
            if ((cand % (seq_t)7 == (seq_t)1) || (cand % (seq_t)7 == (seq_t)4)) {
                if (is_prime_big(cand)) {
                    break;       /* found the next required prime */
                }
            }
            cand += (seq_t)2;   /* keep the candidate odd */
        }

        /* grow array if necessary */
        if (primes_size >= primes_cap) {
            primes_cap = primes_cap * 2 + 1;
            primes = (seq_t*)realloc(primes, primes_cap * sizeof(*primes));
        }

        primes[primes_size++] = cand;
        last_checked = cand;
    }

    return primes[(size_t)n - 1];
}
