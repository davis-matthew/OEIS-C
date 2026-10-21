/*
 * A215169  Primes congruent to {2, 5} mod 17.
 * https://oeis.org/A215169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 2 or 5 modulo 17 (OEIS A215169) by incremental trial division. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static state holding all primes discovered so far and the primes of interest. */
    static seq_t *all_primes = NULL;
    static size_t all_cap = 0, all_len = 0;
    static seq_t *interest  = NULL;
    static size_t interest_cap = 0, interest_len = 0;
    static idx_t last_checked = 2;
    static bool initialized = false;

    /* First call: initialize with prime 2. */
    if (!initialized) {
        seq_t p = (seq_t)2;
        append_candidate(&all_primes,  &all_len,  &all_cap,  p);
        if ((2 % 17) == 2 || (2 % 17) == 5)
            append_candidate(&interest, &interest_len, &interest_cap, p);
        last_checked = 2;
        initialized = true;
    }

    /* Generate primes of interest until we have at least n of them. */
    while (interest_len < (size_t)n) {
        /* Find next candidate. */
        idx_t cand = last_checked + 1;
        if (cand % 2 == 0)   /* skip even numbers >2 */
            cand++;
        seq_t candS = (seq_t)cand;

        /* Test primality by trial division using known primes. */
        bool isPrime = true;
        for (size_t i = 0; i < all_len; i++) {
            seq_t p = all_primes[i];
            seq_t p2 = p * p;
            if (p2 > candS)
                break;
            if (candS % p == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) {
            /* Store the new prime in the list of all primes. */
            append_candidate(&all_primes, &all_len, &all_cap, candS);
            /* If it satisfies the congruence condition, store it in interest. */
            if ((cand % 17) == 2 || (cand % 17) == 5)
                append_candidate(&interest, &interest_len, &interest_cap, candS);
        }

        last_checked = cand;   /* advance for next iteration */
    }

    return interest[(size_t)(n - 1)];
}
