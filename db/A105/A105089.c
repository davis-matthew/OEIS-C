/*
 * A105089  Sum of the primes in ordered 3 X 3 prime squares.
 * https://oeis.org/A105089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p(9n-8) + p(9n-7) + … + p(9n) (sum of a 3×3 prime square)
 * Range:     u8 n <= 1, u16 n <= 103, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_prime_upper_bound, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"
#include "../utils/simple_sieve.h"

/* Compute a(n) for n >= 1.  For n < 1 the function returns 0.          */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* sequence is defined for n ≥ 1 */

    /* We need the first 9*n primes.                         */
    size_t needed = 9 * (size_t)n;

    /* Estimate an upper bound for the needed-th prime.       */
    uint64_t upper = estimate_prime_upper_bound((idx_t)needed);

    /* Generate all primes up to that bound.                   */
    uint32_t *primes = NULL;
    size_t    count  = 0;
    simple_sieve((uint32_t)upper, &primes, &count);

    /* If the estimate was too low, double the bound until we have enough. */
    while (count < needed) {
        upper <<= 1;                 /* double the bound */
        free(primes);
        simple_sieve((uint32_t)upper, &primes, &count);
    }

    /* Sum the block of 9 consecutive primes that corresponds to a(n). */
    size_t start = needed - 9;        /* 0‑based index of the first prime in the block */
    seq_t  sum   = 0;
    for (size_t i = start; i < needed; ++i) {
        sum += (seq_t)primes[i];
    }

    /* Clean up the allocated array. */
    free(primes);

    return sum;
}
