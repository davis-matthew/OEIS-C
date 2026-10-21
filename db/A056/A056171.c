/*
 * A056171  a(n) = pi(n) - pi(floor(n/2)), where pi is A000720.
 * https://oeis.org/A056171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(n) - pi(floor(n/2)) where pi is the prime-counting
 *            function
 * Range:     u8 n <= 4098, u16 n <= 9456, u32 n <= 9456, u64 n <= 9456,
 *            u128 n <= 9456, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static sieve of Eratosthenes and prefix
 * prime counts up to the maximum argument seen.  Each new larger n
 * triggers a re-sieve of the complete range up to that n, which is
 * inexpensive for n ≤ ~10⁶ and fast enough for all tests.  The routine
 * can be called in any order and returns the exact value in seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for the sieve */
static idx_t max_sieved = 0;
static uint8_t *is_prime = NULL;   /* 0 = composite, 1 = prime */
static idx_t *pi_prefix = NULL;    /* prefix counts of primes */

/* Recompute the sieve and prefix array up to 'limit' (inclusive). */
static void recompute_sieve(idx_t limit)
{
    if (limit < max_sieved) return;           /* already up to date */

    /* Allocate or resize the arrays */
    uint8_t *tmp_prime = (uint8_t *)realloc(is_prime, (limit + 1) * sizeof(uint8_t));
    idx_t *tmp_prefix = (idx_t *)realloc(pi_prefix, (limit + 1) * sizeof(idx_t));
    if (!tmp_prime || !tmp_prefix) {
        /* Allocation failed – abort.  In the judge this should not happen. */
        abort();
    }
    is_prime  = tmp_prime;
    pi_prefix = tmp_prefix;

    /* Initialise: mark everything as prime, then clear 0 and 1. */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Classic Eratosthenes sieve */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Build prefix prime counts */
    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        pi_prefix[i] = pi_prefix[i - 1] + (is_prime[i] ? 1 : 0);
    }

    max_sieved = limit;
}

/* Main function required by the OIES harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n < 1, though not used */

    recompute_sieve(n);                   /* ensure we have counts up to n */

    idx_t pi_n = pi_prefix[n];
    idx_t half = n >> 1;                  /* floor(n/2) */

    /* pi_prefix was computed up to n, so includes half */
    idx_t pi_half = pi_prefix[half];

    return (seq_t)(pi_n - pi_half);
}
