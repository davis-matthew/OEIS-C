/*
 * A219586  Greatest prime factor of Product_{x=1..n} (x^2 + 1).
 * https://oeis.org/A219586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A219586: a(n) = greatest prime factor of ∏_{x=1}^n (x^2+1)   */

/* The function computes the result directly for each call.
   It generates all primes up to n with a simple sieve,
   then factors each x^2+1 (for x = 1..n) using these primes,
   keeping track of the largest prime factor seen. */

#include <stdint.h>
#include <stdlib.h>

/* Static helper: create a list of primes up to a given limit.
   The caller owns the returned array and must free it with free(). */
static void sieve_primes(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Mark all numbers as prime initially. */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(limit + 1));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = 0;
    is_prime[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Count primes to allocate the array. */
    size_t len = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++len;
    }

    uint64_t *primes = NULL;
    if (len > 0) {
        primes = (uint64_t*)malloc(len * sizeof(uint64_t));
        if (!primes) exit(1);
        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[i]) {
                primes[idx++] = (uint64_t)i;
            }
        }
    }

    free(is_prime);
    *primes_out = primes;
    *cnt_out = len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset */

    /* Generate primes up to n once per call. */
    uint64_t *primes = NULL;
    size_t primes_len = 0;
    sieve_primes(n, &primes, &primes_len);

    seq_t current_max = 0;

    /* Process each x from 1 to n. */
    for (idx_t x = 1; x <= n; ++x) {
        uint64_t temp = (uint64_t)x * (uint64_t)x + 1;
        uint64_t maxp   = 1;

        /* Factor temp using the precomputed primes. */
        for (size_t pi = 0; pi < primes_len && primes[pi] * primes[pi] <= temp; ++pi) {
            uint64_t p = primes[pi];
            if (temp % p == 0) {
                maxp = p;
                while (temp % p == 0) {
                    temp /= p;
                }
            }
        }
        if (temp > 1) {          /* remaining prime factor */
            maxp = temp;
        }

        if ((seq_t)maxp > current_max) {
            current_max = (seq_t)maxp;
        }
    }

    free(primes);
    return current_max;
}
