/*
 * A074990  Number of primes in the interval (n,3n].
 * https://oeis.org/A074990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(3n) – pi(n) (number of primes in the interval (n,3n])
 * Range:     u8 n <= 962, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a growing sieve to compute prime counts up to the required limit. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;          /* current maximum index sieved */
static uint8_t *is_prime = NULL;       /* 1 if index is prime, 0 otherwise */
static uint32_t *prime_prefix = NULL;  /* prime counts up to each index */

/* extend the sieve to cover at least limit */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* grow arrays to new size */
    is_prime = (uint8_t *)realloc(is_prime, (limit + 1) * sizeof(uint8_t));
    prime_prefix = (uint32_t *)realloc(prime_prefix, (limit + 1) * sizeof(uint32_t));

    /* initialise all entries as prime candidates */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* simple Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p) {
                is_prime[m] = 0;
            }
        }
    }

    /* compute prefix prime counts */
    prime_prefix[0] = 0;
    prime_prefix[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + (is_prime[i] ? 1 : 0);
    }

    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined below offset, but defined as 0 */

    idx_t limit = 3 * n;
    extend_sieve(limit);

    /* Count primes in (n,3n] = primes ≤3n – primes ≤n */
    seq_t count = (seq_t)(prime_prefix[limit] - prime_prefix[n]);

    return count;
}
