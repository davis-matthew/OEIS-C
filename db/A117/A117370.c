/*
 * A117370  Number of primes between smallest prime divisor of n and largest
 *          prime divisor of n.
 * https://oeis.org/A117370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of primes strictly between the smallest and largest
 *            prime divisors of n
 * Range:     u8 n <= 3253, u16 n <= 9336, u32 n <= 9336, u64 n <= 9336,
 *            u128 n <= 9336, big any n
 * Helpers:   smallest_and_largest_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smallest_and_largest_prime_factors.h"

/* --- Cached prime sieve: is_prime[i] indicates primality of i,
   pi_prefix[i] counts primes ≤ i.  --- */
static unsigned char *sieve_is_prime = NULL;
static seq_t *pi_prefix = NULL;
static idx_t sieve_limit = 0;

/* Ensure that the sieve covers all integers up to limit.  */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already enough */
    free(sieve_is_prime);
    free(pi_prefix);
    sieve_is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    pi_prefix      = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!sieve_is_prime || !pi_prefix) return; /* allocation failed */

    for (idx_t i = 0; i <= limit; ++i) sieve_is_prime[i] = 1;
    if (limit >= 0) sieve_is_prime[0] = 0;
    if (limit >= 1) sieve_is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve_is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve_is_prime[j] = 0;
        }
    }
    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        pi_prefix[i] = pi_prefix[i - 1] + (seq_t)sieve_is_prime[i];
    }
    sieve_limit = limit;
}

/* --- Sequence a(n): number of primes strictly between smallest and
   largest prime divisor of n. --- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;            /* a(1) = 0 by definition */

    uint64_t nn   = (uint64_t)n;
    uint64_t spf  = 0;
    uint64_t lpf  = 0;

    smallest_and_largest_prime_factors(nn, &spf, &lpf);

    if (spf >= lpf) return 0;        /* n has only one distinct prime divisor */

    /* Ensure the sieve covers up to lpf-1. */
    idx_t limit = lpf;
    if (limit < 2) limit = 2;
    ensure_sieve(limit);

    /* number of primes in (spf, lpf) = pi(lpf-1) - pi(spf) */
    seq_t count = pi_prefix[(size_t)(lpf - 1)] - pi_prefix[(size_t)spf];
    return count;
}
