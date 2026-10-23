/*
 * A182434  Number of primes p < n such that 4*p+1 is also prime.
 * https://oeis.org/A182434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A182434:
   a(n) = number of primes p <= n such that 4*p+1 is also prime.
   Implementation uses a sieve of Eratosthenes up to 4*n+1. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 2 there are no primes to test. */
    if (n < 2u) return 0;

    /* Limit of the sieve must cover all numbers 4*p+1 for p <= n. */
    idx_t limit = 4 * n + 1;

    /* Allocate sieve array. */
    bool *is_prime = (bool *)malloc((limit + 1u) * sizeof(bool));
    if (!is_prime) return 0;          /* allocation failure */

    /* Initialise: assume all numbers >=2 are prime */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = false;
    if (limit >= 1u) is_prime[1] = false;

    /* Eratosthenes sieve */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= limit; m += p)
            is_prime[m] = false;
    }

    /* Count qualifying primes p <= n. */
    seq_t count = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p] && is_prime[4 * p + 1])
            ++count;
    }

    free(is_prime);
    return count;
}
