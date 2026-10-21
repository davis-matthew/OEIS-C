/*
 * A064799  Sum of n-th prime number and n-th composite number.
 * https://oeis.org/A064799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + composite(n) (offset 1)
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;
static seq_t prime_candidate = 2;

static seq_t *composites = NULL;
static size_t comps_cap = 0;
static size_t comps_len = 0;
static seq_t composite_candidate = 4;

/* Ensure that at least n primes are stored. */
static void ensure_primes(idx_t n) {
    while (primes_len < (size_t)n) {
        while (!is_prime_big(prime_candidate))
            ++prime_candidate;
        if (primes_len == primes_cap) {
            size_t newcap = primes_cap ? primes_cap * 2 : 64;
            primes = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
            primes_cap = newcap;
        }
        primes[primes_len++] = prime_candidate;
        ++prime_candidate;
    }
}

/* Ensure that at least n composite numbers are stored. */
static void ensure_composites(idx_t n) {
    while (comps_len < (size_t)n) {
        while (is_prime_big(composite_candidate))
            ++composite_candidate;
        if (comps_len == comps_cap) {
            size_t newcap = comps_cap ? comps_cap * 2 : 64;
            composites = (seq_t *)realloc(composites, newcap * sizeof(seq_t));
            comps_cap = newcap;
        }
        composites[comps_len++] = composite_candidate;
        ++composite_candidate;
    }
}

/* External entry point of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                         /* offset handling */
    ensure_primes(n);
    ensure_composites(n);
    return primes[n - 1] + composites[n - 1];
}
