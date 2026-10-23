/*
 * A176289  Denominators of the rational sequence with e.g.f.
 *          (x/2)*(1+exp(-x))/(1-exp(-x)).
 * https://oeis.org/A176289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(2m) = product of all primes p ≤ 2m+1 such that (p-1) | (2m);
 *            a(2m+1) = 1. Equivalent to the denominator of the Bernoulli
 *            number B_{2m}
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 179, u64 n <= 539,
 *            u128 n <= 3779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum number of primes we will store. */
/* This is enough for n ≤ 200000 (about 17984 primes). */
#define MAX_PRIME_ARRAY 200000

/* Prime list and bookkeeping. */
static uint64_t primes[MAX_PRIME_ARRAY];
static size_t   prime_cnt   = 0;
static idx_t    prime_max   = 1;

/* Ensure that all primes up to 'limit' are generated and stored. */
static void ensure_primes_up_to(idx_t limit) {
    if (limit <= prime_max) return;

    for (idx_t x = prime_max + 1; x <= limit; ++x) {
        bool is_prime = true;
        /* Test divisibility by known primes up to sqrt(x). */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > x) break;
            if (x % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt < MAX_PRIME_ARRAY) {
                primes[prime_cnt++] = (uint64_t)x;
            }
        }
    }
    prime_max = limit;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* a(0) = 1, and all odd indices give 1. */
    if (n == 0) return (seq_t)1;
    if (n & (idx_t)1) return (seq_t)1;

    /* For even n: compute the denominator of B_{n}. */
    idx_t limit = n + 1;          /* we need primes up to n+1 (since p-1 ≤ n). */
    ensure_primes_up_to(limit);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit) break;
        if (n % ((idx_t)p - 1) == 0) {
            result = result * (seq_t)p;
        }
    }
    return result;
}
