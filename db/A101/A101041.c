/*
 * A101041  Number of numbers not greater than n having no more than two prime
 *          factors.
 * https://oeis.org/A101041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000720(n) + A072000(n) + 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: count primes and semiprimes up to n */
/* This function returns the number of integers <= n that have at most two prime factors
   (counted with multiplicity), i.e. 1 plus the number of primes <= n plus the
   number of semiprimes <= n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0; /* Undefined for n < 1, but assume n>=1 */

    /* Sieve of Eratosthenes up to n */
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_prime)
        return 0; /* Allocation failure, return 0 as a safe fallback */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= n; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes and build a list of primes */
    idx_t prime_count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++prime_count;

    /* Allocate array for primes */
    idx_t *primes = (idx_t *)malloc(prime_count * sizeof(idx_t));
    if (!primes) {
        free(is_prime);
        return 0; /* Allocation failure, return 0 as fallback */
    }

    /* Populate primes array */
    idx_t pidx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            primes[pidx++] = i;

    /* Count semiprimes:
       For each prime p <= q, if p*q <= n, count one semiprime. */
    seq_t semiprime_count = 0;
    for (idx_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        for (idx_t j = i; j < prime_count; ++j) {
            idx_t q = primes[j];
            /* Use idx_t for multiplication to avoid overflow on smaller seq_t */
            if ((idx_t)p * q <= n)
                ++semiprime_count;
            else
                break; /* further q will be larger, so break */
        }
    }

    /* Compute final answer: 1 (for the number 1) + prime_count + semiprime_count */
    seq_t ans = 1;
    ans += (seq_t)prime_count;
    ans += semiprime_count;

    /* Clean up */
    free(is_prime);
    free(primes);

    return ans;
}
