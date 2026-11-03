/*
 * A100394  a(n) is the subscript of the greatest prime factor of (2*prime(n)
 *          + 1).
 * https://oeis.org/A100394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π( largest prime factor of (2*prime(n)+1) )
 * Range:     u8 n <= 31, u16 n <= 4669, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9307 (2026-11-01)
 * Helpers:   is_prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A100394 */

/* Include the required helper functions */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_trial.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

/* Greatest prime factor of an unsigned integer */
static seq_t greatest_prime_factor(seq_t value)
{
    seq_t max_factor = 1;
    seq_t n = value;

    for (seq_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            max_factor = d;
            while (n % d == 0) {
                n /= d;
            }
        }
    }
    if (n > 1) {
        max_factor = n;
    }
    return max_factor;
}

seq_t a(idx_t n)
{
    /* For OEIS the sequence is defined starting at n = 1. */
    if (n < 1)
        return (seq_t)0;

    /* Get the n-th prime q. */
    seq_t q = get_prime((uint64_t)n);          /* q = prime(n) */

    /* Compute m = 2*q + 1. */
    seq_t m = q * 2 + 1;

    /* Find the greatest prime factor r of m. */
    seq_t r = greatest_prime_factor(m);       /* r = A006530(m) */

    /* Now find the index of r among the primes: pi(r). */
    uint64_t idx = 1;                          /* index counter */
    while (1) {
        seq_t p = get_prime(idx);             /* p = prime(idx) */
        if (p == r)                           /* found the prime */
            return (seq_t)idx;                /* a(n) = idx */
        if (p > r)                            /* safety out-of-range guard */
            break;
        ++idx;
    }

    /* In theory we should never reach this point; return 0 as a safeguard. */
    return (seq_t)0;
}
