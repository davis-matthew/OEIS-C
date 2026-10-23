/*
 * A039915  Smallest k such that k(p-1)-1 is positive and divisible by p where
 *          p = n-th prime.
 * https://oeis.org/A039915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039915:
   a(n) = p + (-1)^p, where p is the n-th prime.
   This is the smallest k such that k(p-1)-1 is positive and divisible by p. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Cached list of primes generated incrementally. */
static uint32_t *prime_cache = NULL;
static size_t    prime_count = 0;
static size_t    prime_cap   = 0;

/* Ensure the cache contains at least `need` primes. */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    /* Initialize cache if needed. */
    if (prime_cache == NULL) {
        prime_cap   = 32;
        prime_cache = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (prime_cache == NULL) {
            /* Allocation failed – abort. */
            abort();
        }
    }

    uint32_t candidate = (prime_count == 0 ? 2 : prime_cache[prime_count - 1] + 1);
    for (; prime_count < (size_t)need; candidate++) {
        if (candidate == 2) {
            /* 2 is prime. */
        } else if (candidate % 2 == 0) {
            /* Skip even numbers > 2. */
            continue;
        } else {
            bool is_prime = true;
            for (size_t i = 0; i < prime_count; i++) {
                uint32_t p = prime_cache[i];
                if ((uint64_t)p * (uint64_t)p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (!is_prime)
                continue;
        }

        /* Grow cache if necessary. */
        if (prime_count == prime_cap) {
            prime_cap *= 2;
            uint32_t *tmp = (uint32_t *)realloc(prime_cache, prime_cap * sizeof(uint32_t));
            if (tmp == NULL) {
                /* Allocation failed – abort. */
                abort();
            }
            prime_cache = tmp;
        }
        prime_cache[prime_count++] = candidate;
    }
}

/* External function implementing A039915. */
seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for offset < 1; return 0 as a guard. */
        return (seq_t)0;

    ensure_primes(n);

    seq_t p = (seq_t)prime_cache[n - 1];
    /* Add 1 if p is even (only p=2), else subtract 1. */
    return (p & (seq_t)1) == 0 ? p + (seq_t)1 : p - (seq_t)1;
}
