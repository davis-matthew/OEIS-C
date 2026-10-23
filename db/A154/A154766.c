/*
 * A154766  Numbers k with exactly one even decimal digit in prime(k).
 * https://oeis.org/A154766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_one_even_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A154766: Numbers k with exactly one even decimal digit in prime(k) */
/* The function computes the n-th such k by generating primes
   incrementally and testing the decimal digits of each prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_one_even_digit.h"

seq_t a(idx_t n)
{
    /* Static state across calls */
    static bool initialized = false;
    static uint64_t *primes      = NULL;  /* stored primes */
    static size_t primes_size    = 0;
    static size_t primes_cap     = 0;
    static uint64_t candidate    = 0;      /* next odd number to test */

    static seq_t *indices       = NULL;   /* k-values that satisfy the property */
    static size_t idx_size      = 0;
    static size_t idx_cap       = 0;

    /* One-time initialization */
    if (!initialized) {
        /* Allocate initial storage for primes */
        primes_cap = 32;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_size = 1;
        candidate = 3;  /* start testing from 3 */

        /* Allocate initial storage for indices */
        idx_cap = 32;
        indices = (seq_t *)malloc(idx_cap * sizeof(seq_t));
        idx_size = 0;

        /* The first prime (2) has exactly one even digit, so add k=1 */
        indices[idx_size++] = (seq_t)1;

        initialized = true;
    }

    /* Generate primes until we have at least n qualifying indices */
    while (idx_size < (size_t)n) {
        /* Test whether candidate is prime */
        bool is_prime = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Append new prime to list */
            if (primes_size == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                primes = tmp;
            }
            primes[primes_size++] = candidate;

            /* Check digit property of the prime itself */
            if (has_one_even_digit((seq_t)candidate)) {
                /* Store the 1‑based index of this prime */
                if (idx_size == idx_cap) {
                    idx_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(indices, idx_cap * sizeof(seq_t));
                    indices = tmp;
                }
                indices[idx_size++] = (seq_t)primes_size;
            }
        }

        ++candidate;  /* next odd candidate */
    }

    return indices[n - 1];
}
