/*
 * A077463  Number of primes p such that n < p < 2n-2.
 * https://oeis.org/A077463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2050, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = number of primes p such that n < p < 2n-2.
   Implements an incremental Sieve of Eratosthenes with a static cache
   that expands only when a larger upper bound is required. */
#include <stdint.h>
#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Trivial small indices: no primes in the required interval. */
    if (n < 4) return (seq_t)0;

    /* Upper bound of the interval (strictly less than). */
    idx_t upper = 2 * n - 2;               /* upper is exclusive */

    /* Static cache of the sieve.  The array holds one byte per integer
       from 0 to cached_limit, where 1 denotes prime.  The cache is
       enlarged only when a larger 'upper' is requested. */
    static size_t cached_limit = 0;        /* size of the allocated array */
    static uint8_t *is_prime = NULL;   /* sieve bitmap */

    /* Rebuild the sieve if a larger bound is required. */
    if (upper > cached_limit) {
        /* Allocate new sieve array.  Cast allocated void* for C++ compatibility. */
        uint8_t *new_sieve = (uint8_t *)malloc((size_t)(upper + 1));
        if (!new_sieve) return (seq_t)0;   /* allocation failed – fallback */

        /* Initialise all entries as potentially prime. */
        for (size_t i = 0; i <= (size_t)upper; ++i)
            new_sieve[i] = 1;
        if (upper >= 0) new_sieve[0] = 0;
        if (upper >= 1) new_sieve[1] = 0;

        /* Eratosthenes Sieve of Eratosthenes up to 'upper'. */
        for (idx_t p = 2; p <= upper / p; ++p) {
            if (new_sieve[p]) {
                for (idx_t m = p * p; m <= upper; m += p)
                    new_sieve[m] = 0;
            }
        }

        /* Replace the old sieve with the new one. */
        if (is_prime) free(is_prime);
        is_prime = new_sieve;
        cached_limit = (size_t)upper;
    }

    /* Count primes in the open interval (n, upper). */
    idx_t count = 0;
    for (idx_t i = n + 1; i < upper; ++i)
        if (is_prime[i]) ++count;

    return (seq_t)count;
}
