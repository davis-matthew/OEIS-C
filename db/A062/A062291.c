/*
 * A062291  Primes p = prime(k) such that k*p - 1 is also a prime.
 * https://oeis.org/A062291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th prime p for which the index k of p satisfies k *
 *            p – 1 is also prime. The sequence is indexed starting with a(1)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 13, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t); */

static seq_t cache[200000];
static size_t cache_len = 0;

/* current_prime holds the candidate prime being examined;
   current_index is its index among all primes (starting at 1). */
static uint64_t current_prime = 2;
static idx_t    current_index = 1;

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        /* Test current_prime; if it is prime we consider it a candidate. */
        if (is_prime(current_prime)) {
            /* k*p – 1 */
            seq_t test = (seq_t)current_index * (seq_t)current_prime - 1;
            if (is_prime_big(test)) {
                cache[cache_len++] = current_prime;
            }
            ++current_index;  /* advance the prime index after a prime */
        }

        /* Advance to the next odd number (next prime candidate). */
        if (current_prime == 2) {
            current_prime = 3;
        } else {
            current_prime += 2;
        }
    }

    return cache[(size_t)n - 1];
}
