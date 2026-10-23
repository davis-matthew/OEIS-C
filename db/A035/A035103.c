/*
 * A035103  Number of 0's in binary representation of n-th prime.
 * https://oeis.org/A035103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of 0's in the binary representation of the n-th
 *            prime. Implementation: incremental prime generation with trial
 *            division and bit counting
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of primes up to the maximum n seen so far */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;
    static uint32_t next_candidate = 3;  /* next odd number to test */

    /* Initialise cache on first call */
    if (primes == NULL) {
        primes_cap = 128;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    }

    if (primes_cnt == 0) {
        primes[0] = 2;
        primes_cnt = 1;
    }

    /* Generate primes until we have at least n of them */
    while (primes_cnt < n) {
        uint32_t p = next_candidate;
        bool is_prime = true;

        /* Check divisibility by stored primes up to sqrt(p) */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t prime = primes[i];
            if ((seq_t)prime * (seq_t)prime > p) break;
            if (p % prime == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = p;
        }

        next_candidate += 2; /* only odd numbers are candidates (except 2) */
    }

    /* Retrieve the n-th prime */
    uint32_t prime_val = primes[n - 1];

    /* Count zeros in its binary representation (minimal form) */
    seq_t zeros = 0;
    uint32_t x = prime_val;
    while (x) {
        if ((x & 1) == 0) zeros++;
        x >>= 1;
    }

    return zeros;
}
