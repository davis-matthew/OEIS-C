/*
 * A080370  Number of distinct values among the differences prime(j)-prime(i),
 *          1 <= i < j <= n.
 * https://oeis.org/A080370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values among all prime differences
 *            prime(j) − prime(i), 1 ≤ i < j ≤ n. The value is ≤ n·(n+1)/2
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   estimate_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/estimate_nth_prime.h"

static uint64_t *sieve_primes(idx_t n, idx_t *count_out) {
    /* Generate first n primes using a simple sieve.
       Memory is freed by the caller. */
    uint64_t limit = estimate_nth_prime(n);
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return NULL;
    for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = true;

    uint64_t *primes = (uint64_t*)malloc(n * sizeof(uint64_t));
    if (!primes) { free(is_prime); return NULL; }
    idx_t idx = 0;
    for (uint64_t i = 2; i <= limit && idx < n; ++i) {
        if (is_prime[i]) {
            primes[idx++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
            }
        }
    }
    free(is_prime);
    *count_out = idx;
    return primes;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* a(1) = 0, a(0) undefined but return 0 */

    /* obtain the first n primes */
    idx_t prime_count = 0;
    uint64_t *primes = sieve_primes(n, &prime_count);
    if (prime_count != n) {
        free(primes);
        return 0;                 /* fallback */
    }

    /* maximum possible difference */
    uint64_t max_diff = primes[n-1] - primes[0];
    bool *seen_diff = (bool*)calloc(max_diff + 1, sizeof(bool));
    if (!seen_diff) {
        free(primes);
        return 0;
    }

    /* count distinct differences */
    idx_t distinct = 0;
    for (idx_t i = 0; i < n-1; ++i) {
        for (idx_t j = i+1; j < n; ++j) {
            uint64_t diff = primes[j] - primes[i];
            if (!seen_diff[diff]) {
                seen_diff[diff] = true;
                ++distinct;
            }
        }
    }

    free(primes);
    free(seen_diff);
    return (seq_t)distinct;
}
