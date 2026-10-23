/*
 * A133541  Sum of fifth powers of five consecutive primes.
 * https://oeis.org/A133541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{4} prime(n+i)^5 (sum of fifth powers of five
 *            consecutive primes)
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for prime numbers. */
static uint64_t *prime_cache      = NULL;
static size_t   prime_cache_cnt   = 0;
static size_t   prime_cache_cap   = 0;

/* Ensure that at least 'needed' primes are stored in prime_cache. */
static void ensure_primes_count(idx_t needed)
{
    /* Nothing to do if we already have enough primes. */
    if (prime_cache_cnt >= needed)
        return;

    /* Grow the cache if required. */
    if (prime_cache_cap < needed) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap : 16;
        while (new_cap < needed)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        prime_cache   = tmp;
        prime_cache_cap = new_cap;
    }

    /* Generate primes until we reach the desired count. */
    uint64_t candidate = prime_cache_cnt ? prime_cache[prime_cache_cnt - 1] + 1 : 2;
    while (prime_cache_cnt < needed) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cache_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_cache_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Sequence entry. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* Undefined for n < 1, return 0 for safety. */

    /* We require primes from n to n+4 inclusive. */
    idx_t needed = n + 4;         /* primes needed so that indices n..n+4 exist */
    ensure_primes_count(needed);

    seq_t sum = 0;
    for (idx_t i = 0; i < 5; ++i) {
        /* Retrieve the (n+i)-th prime (1‑based). */
        uint64_t pu = prime_cache[(size_t)(n - 1 + i)];
        seq_t p = (seq_t)pu;

        /* Compute p^5 using repeated multiplication. */
        seq_t p5 = p;
        p5 *= p;   /* p^2 */
        p5 *= p;   /* p^3 */
        p5 *= p;   /* p^4 */
        p5 *= p;   /* p^5 */

        sum += p5;
    }
    return sum;
}
