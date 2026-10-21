/*
 * A090433  Primes p(k) having a smaller sum of digits than k.
 * https://oeis.org/A090433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p(k) for which digit_sum(p(k)) <
 *            digit_sum(k), where k is the index of p(k) in the sequence of
 *            all primes
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   push_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/push_uint64.h"

/* Sum of decimal digits of a 64‑bit unsigned integer. */
static inline uint64_t digit_sum_u64(uint64_t x)
{
    uint64_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Trial‑division primality test using previously found primes. */
static int is_prime_uint64(uint64_t n,
                           const uint64_t *primes,
                           size_t primes_len)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < primes_len; i++) {
        uint64_t d = primes[i];
        if (d * d > n) break;
        if (n % d == 0) return 0;
    }
    return 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Static state for incremental computation. */
    static uint64_t *primes_all      = NULL;
    static size_t   primes_all_len   = 0;
    static size_t   primes_all_cap   = 0;

    static uint64_t *satisfied       = NULL;
    static size_t   satisfied_len    = 0;
    static size_t   satisfied_cap    = 0;

    static uint64_t current_prime_candidate = 2;   /* next integer to test */
    static uint64_t k = 0;                         /* index of the current prime among all primes */

    /* Initialise the dynamic arrays on first call. */
    if (satisfied_cap == 0) {
        primes_all   = NULL;
        primes_all_cap = 0;
        satisfied    = NULL;
        satisfied_cap = 0;
        push_uint64(&primes_all, &primes_all_len, &primes_all_cap, 2); /* first prime */
        k = 1;
        if (digit_sum_u64(2) < digit_sum_u64(1))
            push_uint64(&satisfied, &satisfied_len, &satisfied_cap, 2);
        current_prime_candidate = 3;
    }

    /* Generate primes until we have found the nth satisfying prime. */
    while (satisfied_len < (size_t)n) {
        uint64_t cand = current_prime_candidate++;
        if (is_prime_uint64(cand, primes_all, primes_all_len)) {
            /* Save the new prime for future trial divisions. */
            push_uint64(&primes_all, &primes_all_len, &primes_all_cap, cand);
            k++; /* increment index of this prime */
            if (digit_sum_u64(cand) < digit_sum_u64(k)) {
                push_uint64(&satisfied, &satisfied_len, &satisfied_cap, cand);
            }
        }
    }

    return (seq_t)satisfied[(size_t)n - 1];
}
