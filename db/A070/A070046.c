/*
 * A070046  Number of primes between prime(n) and 2*prime(n) exclusive.
 * https://oeis.org/A070046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pi(2*prime(n)) - n
 * Range:     u8 n <= 311, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/sieve_primes.h"

/* Static cache for primes up to a current limit. */
static idx_t *g_primes = NULL;      /* Array of primes  <= g_limit */
static size_t g_prime_cnt = 0;      /* Number of primes in g_primes */
static idx_t g_limit = 0;           /* Current sieve limit */

/* Ensure the sieve contains all primes up to 'limit'.
   If not, recompute the sieve. */
static void ensure_sieve(idx_t limit)
{
    if (g_limit >= limit)
        return;

    /* Free old array if allocated */
    free(g_primes);
    g_primes = NULL;
    g_prime_cnt = 0;

    /* Recompute sieve up to new limit */
    sieve_primes(limit, &g_primes, &g_prime_cnt);
    g_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Requirement: n >= 1 for the defined sequence */
    if (n == 0)
        return 0;           /* Undefined for offset 0, but return 0 safely */

    /* Get the n-th prime (prime(n)) */
    uint32_t p = get_prime(n);          /* get_prime returns uint32_t */

    /* Compute limit = 2 * prime(n) */
    idx_t limit = (idx_t)p * 2ULL;

    /* Ensure sieve covers all primes up to limit */
    ensure_sieve(limit);

    /* primepi(limit) == number of primes <= limit == g_prime_cnt */
    seq_t pi_val = (seq_t)g_prime_cnt;

    /* Result: pi(2*prime(n)) - n */
    return pi_val - (seq_t)n;
}
