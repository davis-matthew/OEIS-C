/*
 * A076555  Greatest prime divisor of n-th prime + 2.
 * https://oeis.org/A076555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest_prime_factor(p_n + 2) where p_n is the n-th prime
 *            (a(1) = 2)
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Dynamic prime table and its bookkeeping. */
static uint64_t *prime_table = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

/* Ensure that prime_table contains at least n primes. */
static void
ensure_primes(size_t n)
{
    while (prime_cnt < n) {
        /* Find the next prime after the last stored one. */
        uint64_t candidate = (prime_cnt == 0) ? 2 : prime_table[prime_cnt - 1] + 1;
        for ( ;; ++candidate ) {
            bool is_prime = true;
            /* Trial division by known primes up to sqrt(candidate). */
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_table[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (prime_cnt == prime_cap) {
                    prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                    prime_table = (uint64_t*)realloc(prime_table, prime_cap * sizeof(uint64_t));
                }
                prime_table[prime_cnt++] = candidate;
                break;
            }
        }
    }
}

/* Return the largest prime divisor of m (m > 1). */
static seq_t
largest_prime_factor(uint64_t m)
{
    uint64_t largest = 0;
    for (size_t i = 0; i < prime_cnt && prime_table[i] * prime_table[i] <= m; ++i) {
        uint64_t p = prime_table[i];
        while (m % p == 0) {
            largest = p;
            m /= p;
        }
    }
    if (m > 1)
        largest = m;   /* remaining factor is prime */
    return (seq_t)largest;
}

/* Public API required by the exercise. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, return 0 for safety */

    ensure_primes((size_t)n);
    uint64_t nth_prime = prime_table[n - 1];
    uint64_t value     = nth_prime + 2;
    return largest_prime_factor(value);
}
