/*
 * A122136  Numerator of Sum[ Prime[k]^2, {k,1,n}] / Product[ Prime[k],
 *          {k,1,n}] = Numerator[ A024450[n] / A002110[n] ].
 * https://oeis.org/A122136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ Sum[ Prime[k]^2, {k,1,n}] / Product[ Prime[k],
 *            {k,1,n}] ]
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;
static size_t prime_cache_len = 0;

/* Ensure that prime_cache contains the first n primes. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_cache_len)
        return;
    /* Allocate a new buffer of the first n primes. */
    uint32_t *new_primes = NULL;
    size_t new_len = 0;
    sieve_first_n_primes(n, &new_primes, &new_len);
    /* Replace the old cache. */
    free(prime_cache);
    prime_cache = new_primes;
    prime_cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, undefined for 0 but safe. */

    ensure_primes(n);

    seq_t sum = 0;
    /* Compute the sum of the squares of the first n primes. */
    for (size_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)prime_cache[i];
        sum += p * p;
    }

    /* Compute the product of the primes that divide 'sum'. */
    seq_t g = 1;
    for (size_t i = 0; i < n; ++i) {
        seq_t p = (seq_t)prime_cache[i];
        if ((sum % p) == 0)
            g *= p;
    }

    /* The numerator of the reduced fraction is sum / gcd(sum, product). */
    return sum / g;
}
