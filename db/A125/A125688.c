/*
 * A125688  Number of partitions of n into three distinct primes.
 * https://oeis.org/A125688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{ p < q < r primes | p + q + r = n }
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Compute a(n): number of partitions of n into three distinct primes. */
seq_t a(idx_t n)
{
    /* Minimum possible sum of three distinct primes is 2 + 3 + 5 = 10. */
    if (n < 10) {
        return (seq_t)0;
    }

    /* Cast idx_t to size_t for array sizes. */
    size_t N = (size_t)n;

    /* Allocate a boolean array for the sieve (cast the void* to bool*). */
    bool *is_prime = (bool *)calloc(N + 1, sizeof(bool));
    if (!is_prime) {
        return (seq_t)0;   /* allocation failed */
    }

    /* Sieve of Eratosthenes: mark all numbers >= 2 as potential primes. */
    for (size_t i = 2; i <= N; ++i) {
        is_prime[i] = true;
    }
    for (size_t p = 2; p * p <= N; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= N; m += p) {
                is_prime[m] = false;
            }
        }
    }

    /* Count the primes up to N to determine the array size. */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            ++prime_cnt;
        }
    }
    if (prime_cnt < 3) {
        free(is_prime);
        return (seq_t)0;
    }

    /* Allocate an integer array to hold the primes (cast the void*). */
    uint64_t *primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }

    /* Populate the primes array. */
    size_t idx = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes[idx++] = (uint64_t)i;
        }
    }

    /* Count triples p < q < r with p + q + r == n. */
    seq_t count = 0;
    for (size_t i = 0; i + 2 < prime_cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i + 1; j + 1 < prime_cnt; ++j) {
            uint64_t q = primes[j];
            uint64_t sum_pq = p + q;
            if (sum_pq >= n) {
                /* further j will only increase q, so break. */
                break;
            }
            uint64_t r = n - sum_pq;
            if (r <= q) {
                continue;  /* need r > q to maintain distinctness and ordering. */
            }
            if (r <= N && is_prime[(size_t)r]) {
                ++count;
            }
        }
    }

    free(primes);
    free(is_prime);
    return count;
}
