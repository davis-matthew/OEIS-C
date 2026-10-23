/*
 * A158203  Primes the sum of whose first digit and largest digit is a prime.
 * https://oeis.org/A158203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1890, u32 n <= 1891, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158203: primes whose sum of the first and largest decimal digit is prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"       /* is_prime_big(seq_t n) */

#ifndef MAX_TERMS
/* maximum number of terms that will be stored during the run */
#define MAX_TERMS 20000
#endif

/* static cache of found terms */
static seq_t cache[MAX_TERMS];
static size_t cache_size = 0;

/* next integer to test for primality; stays in the global state */
static seq_t next_candidate = 2;

/* true if the sum of the first and largest digit of n is a prime number (≤ 18) */
static inline bool is_good_prime(seq_t n)
{
    /* first digit */
    seq_t tmp = n;
    while (tmp >= (seq_t)10) {
        tmp /= (seq_t)10;
    }
    uint64_t first = (uint64_t)tmp;

    /* largest digit */
    uint8_t largest = 0;
    tmp = n;
    while (tmp > (seq_t)0) {
        uint8_t d = (uint8_t)((uint64_t)(tmp % (seq_t)10));
        if (d > largest) largest = d;
        tmp /= (seq_t)10;
    }

    uint64_t sum = first + largest;

    /* primes up to 18 */
    switch (sum) {
        case 2: case 3: case 5: case 7: case 11: case 13: case 17:
            return true;
        default:
            return false;
    }
}

/* Returns the n-th term of A158203.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined */

    /* Ensure we have cached at least n terms. */
    while (cache_size < (size_t)n) {
        /* Find the next prime starting from next_candidate. */
        seq_t p = next_candidate;
        while (!is_prime_big(p)) {
            /* Skip even numbers after 2. */
            if (p == (seq_t)2) {
                p = (seq_t)3;
            } else {
                p += (seq_t)2;
            }
        }

        /* Advance the global candidate for the next search. */
        if (p == (seq_t)2) {
            next_candidate = (seq_t)3;
        } else {
            next_candidate = p + (seq_t)2;
        }

        /* Store if the prime satisfies the sequence condition. */
        if (is_good_prime(p)) {
            if (cache_size < MAX_TERMS) {
                cache[cache_size++] = p;
            } else {
                /* Term count exceeds pre‑allocated cache.  We simply
                   discard further terms; this should not happen for the
                   ranges used in the tests. */
                break;
            }
        }
    }

    return cache[(size_t)n - 1];
}
