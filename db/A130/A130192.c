/*
 * A130192  Smallest k with k*nextPrime(n) > nextPrime(k*n).
 * https://oeis.org/A130192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130192: a(n) = smallest k such that k*nextPrime(n) > nextPrime(k*n) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* n must be >= 1; return 0 for invalid inputs */
    if (n < 1) return 0;

    /* Compute the next prime after n */
    seq_t p_n = (seq_t)next_prime((uint64_t)n);

    /* Search for the smallest k satisfying the condition */
    for (seq_t k = 1;; ++k) {
        uint64_t prod = (uint64_t)k * (uint64_t)n;          /* k * n */
        uint64_t pk_n = next_prime(prod);                  /* next prime after k*n */
        if ((uint64_t)k * (uint64_t)p_n > pk_n)            /* k * p_n > pk_n ? */
            return k;
    }
}
