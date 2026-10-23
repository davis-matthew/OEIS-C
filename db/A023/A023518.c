/*
 * A023518  Greatest exponent in prime-power factorization of
 *          prime(n)*prime(n-1) - 1; a(1) = 0.
 * https://oeis.org/A023518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest exponent in the prime‐power factorization * of
 *            prime(n) * prime(n-1) - 1 (with a(1) = 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t k) */

/* maximum prime needed to factor prime(n)*prime(n-1)-1 for n <= 10000
   prime(10000) ≈ 104729, product ≈ 10^10, sqrt ≈ 100k.  We sieve up to 200k
   to be safe. */
#define PRIME_SIEVE_LIMIT 200000

static uint32_t prime_list[PRIME_SIEVE_LIMIT + 1];
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Simple linear sieve of Eratosthenes. */
static void init_primes(void) {
    if (primes_initialized) return;
    memset(prime_list, 0, sizeof(prime_list));
    bool is_composite[PRIME_SIEVE_LIMIT + 1];
    memset(is_composite, 0, sizeof(is_composite));

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            prime_list[prime_count++] = (uint32_t)i;
            if (i * i <= PRIME_SIEVE_LIMIT) {
                for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    primes_initialized = true;
}

/* Compute the max exponent of any prime factor of x (x >= 2). */
static seq_t max_factor_exponent(uint64_t x) {
    seq_t max_exp = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= x; ++i) {
        uint32_t p = prime_list[i];
        if (x % p == 0) {
            seq_t exp = 0;
            while (x % p == 0) {
                ++exp;
                x /= p;
            }
            if (exp > max_exp) max_exp = exp;
        }
    }
    if (x > 1) {            /* remaining prime factor */
        if (1 > max_exp) max_exp = 1;
    }
    return max_exp;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    init_primes();

    uint64_t p_n     = (uint64_t)get_prime(n);
    uint64_t p_prev  = (uint64_t)get_prime(n - 1);
    uint64_t product = p_n * p_prev - 1;

    return max_factor_exponent(product);
}
