/*
 * A155215  a(n) is the average of the smallest prime greater than n and
 *          largest prime smaller than n + prime(n).
 * https://oeis.org/A155215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes a(n) = (s(n) + l(n))/2 where * s(n) = smallest prime >
 *            n, * l(n) = largest prime < n + p(n), and * p(n) = nth prime.
 *            The implementation uses a cached * table of primes built up to
 *            the largest required bound. * It maintains the table across
 *            calls and expands it * lazily as larger n are re...
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_table = NULL;     /* array of primes */
static size_t    prime_count = 0;        /* number of primes stored */
static idx_t     prime_limit = 0;        /* maximum prime value stored */

/* Ensure prime_table contains all primes <= limit.  Allocates
 * a new array and replaces the old one if needed. */
static void ensure_prime_table(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_table != NULL)
        free(prime_table);

    build_primes_upto(limit, &prime_table, &prime_count);
    prime_limit = limit;
}

/* Binary search: first index of prime > value. */
static size_t first_prime_gt(uint64_t value)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] > value)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* may equal prime_count */
}

/* Binary search: first index of prime >= value. */
static size_t first_prime_ge(uint64_t value)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] >= value)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* may equal prime_count */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n = 0; return 0 as a safe default */
        return 0;

    /* Rough upper bound for the nth prime (valid for n ≥ 6) is n*20.
     * For small n we use a minimum bound of 100. */
    idx_t estimate = n * 20;
    if (estimate < 100)
        estimate = 100;

    ensure_prime_table(estimate);

    /* If we don't yet have n primes, double the limit until we do. */
    while (prime_count < (size_t)n) {
        estimate = (estimate * 2 > estimate) ? estimate * 2 : estimate + 1;
        ensure_prime_table(estimate);
    }

    /* nth prime (1‑based index). */
    uint64_t p_n = prime_table[(size_t)(n - 1)];

    /* Calculate largest needed prime bound: n + p_n. */
    uint64_t needed = (uint64_t)n + p_n;
    if ((idx_t)needed > prime_limit) {
        idx_t new_limit = (idx_t)needed;
        /* Ensure the limit grows at least doubly to avoid repeated resizes. */
        while (new_limit <= (idx_t)needed)
            new_limit = new_limit * 2;
        ensure_prime_table(new_limit);
    }

    /* Smallest prime strictly greater than n. */
    size_t idx_sp = first_prime_gt((uint64_t)n);
    uint64_t sp = (idx_sp < prime_count) ? prime_table[idx_sp] : 0;

    /* Largest prime strictly less than n + p(n). */
    size_t idx_lg = first_prime_ge(needed);
    uint64_t lp = (idx_lg == 0) ? 0 : prime_table[idx_lg - 1];

    /* Compute the average, truncating towards zero as per integer division. */
    seq_t sum = (seq_t)sp + (seq_t)lp;
    return sum / 2;
}
