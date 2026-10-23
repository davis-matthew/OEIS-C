/*
 * A036541  Deficit of central binomial coefficients in terms of number of
 *          prime factors: a(n) shows how many fewer prime factors the n-th
 *          central binomial coefficient has than n!.
 * https://oeis.org/A036541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime_pi(n) - omega(binomial(n, floor(n/2)))
 * Range:     u8 n <= 6092, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_pi, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega counts distinct prime factors. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/sieve_primes.h"
#include "../utils/prime_pi.h"

/* Static cache of primes up to the maximum n requested. */
static idx_t primes_max = 0;
static idx_t *primes = NULL;
static size_t prime_count = 0;

/* Ensure that primes list is computed up to at least n. */
static void ensure_primes(idx_t n)
{
    if (n <= primes_max) return;

    /* Rebuild the list.  Free the old one if present. */
    if (primes) free(primes);

    idx_t *tmp = NULL;
    size_t cnt = 0;
    sieve_primes(n, &tmp, &cnt);
    primes   = tmp;
    prime_count = cnt;
    primes_max = n;
}

/* Return exponent of prime p in n!. */
static idx_t exp_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Main function: OEIS A036541. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Make sure we have all primes up to n. */
    ensure_primes(n);

    /* Compute the number of distinct prime factors of C(n, floor(n/2)). */
    idx_t k = n / 2;          /* floor(n/2) */
    idx_t distinct = 0;

    for (size_t i = 0; i < prime_count && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t e = exp_factorial(n,    p)
                - exp_factorial(k,     p)
                - exp_factorial(n - k, p);
        if (e > 0) ++distinct;
    }

    /* prime_pi returns the number of primes ≤ n. */
    seq_t pi_n = prime_pi(n);

    /* Result is the difference. */
    return (seq_t)(pi_n - distinct);
}
