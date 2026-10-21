/*
 * A074393  Number of primes between sigma(n) and phi(n) exclusive, minus 1 if
 *          sigma(n) is a prime.
 * https://oeis.org/A074393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #primes q with σ(n) < q < φ(n) (exclusive), minus 1 if
 *            σ(n) is prime
 * Range:     u8 n <= 29, u16 n <= 8421, u32 n <= 9674, u64 n <= 9674,
 *            u128 n <= 9674, big any n
 * Helpers:   floor_sqrt, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/floor_sqrt.h"

static idx_t sievelimit = 0;
static uint8_t *is_prime_sieve = NULL;
static seq_t *prime_prefix = NULL;

/* Build a sieve of Eratosthenes up to at least 'limit', inclusive. */
static void build_sieve(idx_t limit)
{
    if (limit <= sievelimit)
        return;

    size_t sz = (size_t)(limit + 1);
    uint8_t *new_sieve = (uint8_t *)malloc(sz);
    if (!new_sieve)
        abort();

    for (idx_t i = 0; i <= limit; ++i)
        new_sieve[i] = 1;
    new_sieve[0] = new_sieve[1] = 0;

    idx_t maxp = (idx_t)floor_sqrt((seq_t)limit);
    for (idx_t p = 2; p <= maxp; ++p) {
        if (new_sieve[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_sieve[m] = 0;
        }
    }

    seq_t *new_prefix = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_prefix)
        abort();

    new_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_prefix[i] = new_prefix[i - 1] + new_sieve[i];

    free(is_prime_sieve);
    free(prime_prefix);

    is_prime_sieve = new_sieve;
    prime_prefix  = new_prefix;
    sievelimit   = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* out of defined range; return 0 */
        return (seq_t)0;

    seq_t sigma = sigma_of(n);
    seq_t phi_n = phi(n);

    seq_t low  = (sigma < phi_n) ? sigma : phi_n;
    seq_t high = (sigma < phi_n) ? phi_n : sigma;

    /* Convert to index types; all values are <= max(sigma,phi). */
    idx_t low_i  = (idx_t)low;
    idx_t high_i = (idx_t)high;

    /* Ensure sieve covers the interval endpoints. */
    if (high_i > sievelimit)
        build_sieve(high_i);

    /* Count primes strictly between low and high. */
    seq_t count = 0;
    if (low_i + 1 <= high_i - 1) {
        count = prime_prefix[high_i - 1] - prime_prefix[low_i];
    }

    /* Subtract 1 if sigma(n) is prime. */
    bool sigma_prime = false;
    if (low_i == (idx_t)sigma)          /* sigma is the lower endpoint */
        sigma_prime = is_prime_sieve[low_i];
    else if (high_i == (idx_t)sigma)    /* sigma is the upper endpoint */
        sigma_prime = is_prime_sieve[high_i];

    if (sigma_prime && count > 0)
        count -= 1;

    return count;
}
