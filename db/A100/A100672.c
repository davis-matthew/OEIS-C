/*
 * A100672  a(1) = 1; thereafter, a(n) = 1 if n-th prime is 3 mod 4, 0 if n-th
 *          prime is 1 mod 4.
 * https://oeis.org/A100672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 1 if the n-th prime is 3 (mod 4), otherwise 0 (with a(1)
 *            = 1 by definition). The n-th prime is found by generating primes
 *            incrementally with trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 regardless of the prime 2 */
    if (n == 1)
        return 1;

    /* Static storage for generated primes */
    static uint64_t *primes = NULL;
    static idx_t prime_count = 0;   /* number of primes stored */
    static idx_t prime_capacity = 0;/* allocated capacity of primes array */
    static uint64_t last_candidate = 2; /* next number to test for primality */

    /* Grow the primes array when necessary */
    if (prime_capacity == 0) {
        prime_capacity = 256;
        primes = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        if (!primes) abort();
    }

    /* Generate primes until we reach n of them */
    while (prime_count < n) {
        /* Find next candidate that might be prime */
        while (1) {
            int is_prime = 1;
            uint64_t limit = 0;
            for (idx_t i = 0; i < prime_count; ++i) {
                uint64_t p = primes[i];
                if (p > last_candidate / p) break;
                limit = last_candidate / p;
                break;
            }

            /* Test divisibility by all stored primes up to sqrt(candidate) */
            for (idx_t i = 0; i < prime_count; ++i) {
                uint64_t p = primes[i];
                if (p > limit) break;
                if (last_candidate % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) break;
            ++last_candidate;
        }

        /* Store the newly found prime */
        if (prime_count == prime_capacity) {
            prime_capacity *= 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, prime_capacity * sizeof(uint64_t));
            if (!tmp) abort();
            primes = tmp;
        }
        primes[prime_count++] = last_candidate;
        ++last_candidate;
    }

    uint64_t p = primes[n - 1];          /* nth prime, 1‑based index */
    return (p & 3ULL) == 3ULL ? 1 : 0; /* 1 if p ≡ 3 (mod 4), else 0 */
}
