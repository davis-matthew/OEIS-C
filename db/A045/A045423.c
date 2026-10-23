/*
 * A045423  Primes congruent to {0, 3, 6} mod 7.
 * https://oeis.org/A045423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045423:
   Primes congruent to {0, 3, 6} modulo 7.
   The implementation searches primes incrementally and stores a
   growing list; it is lazy and works for any call order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static cache of primes that satisfy the residue condition.
   The array is lazily resized as more primes are discovered. */
static seq_t *primes = NULL;
static size_t primes_cap = 0;           /* current allocation capacity   */
static size_t primes_len = 0;           /* number of valid entries        */
static idx_t   scan_candidate = 3;      /* integer to scan for next prime */

/* Return the n-th prime such that prime % 7 is 0, 3, or 6.
   Uses zero‑based indexing for the internal list; n starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence is defined only for n ≥ 1, but return 0 for safety */
        return (seq_t)0;

    /* Grow the prime list until it contains at least n elements. */
    while (primes_len < (size_t)n) {
        /* Find the next prime with the required residue. */
        while (true) {
            idx_t mod = scan_candidate % 7;
            if ((mod == 0 || mod == 3 || mod == 6) &&
                is_prime_big((seq_t)scan_candidate)) {
                /* Ensure sufficient capacity for the new prime. */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap == 0 ? 4 : primes_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* In a real build we should handle out‑of‑memory elegantly. */
                        exit(EXIT_FAILURE);
                    }
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = (seq_t)scan_candidate;
                break;                 /* exit the inner search loop */
            }
            ++scan_candidate;          /* try the next integer */
        }
        ++scan_candidate;              /* advance past the found prime */
    }

    return primes[(size_t)n - 1];
}
