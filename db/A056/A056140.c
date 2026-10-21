/*
 * A056140  a(n) = n^2 - previousprime(n)*nextprime(n), for n>2.
 * https://oeis.org/A056140
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^2 - previousprime(n)*nextprime(n) (for n > 2, offset 3)
 * Range:     i8 n <= 28, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic prime sieve (utils/build_primes.h) to
   find the previous and next primes.  The computed value fits in the
   seq_t type; intermediate products are also computed in seq_t.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Static state: prime array and its current memory limit. */
static idx_t *prime_array = NULL;   /* sorted list of primes */
static size_t prime_count = 0;      /* number of stored primes   */
static idx_t current_limit = 0;     /* the maximum number sieved */

/* Ensure the prime table covers at least [0..limit].  Rebuilds the
   table only when necessary. */
static void ensure_primes_atleast(idx_t limit)
{
    if (current_limit >= limit)
        return;

    /* Determine the new sieve limit. */
    idx_t new_limit = current_limit ? current_limit * 2 : 1000;
    if (new_limit < limit)
        new_limit = limit;

    /* Build the new prime list.  Old memory is freed automatically by
       build_primes (it reallocates).  We free the old array to avoid leaks. */
    idx_t *new_array = NULL;
    size_t new_count = 0;
    build_primes(new_limit, &new_array, &new_count);

    free(prime_array);
    prime_array = new_array;
    prime_count = new_count;
    current_limit = new_limit;
}

/* Binary search: largest prime < n  (returns 0 if none).  */
static idx_t prev_prime(idx_t n)
{
    if (n <= 2)
        return 0;
    ensure_primes_atleast(n);
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_array[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)
        return 0;
    return prime_array[lo - 1];
}

/* Binary search: smallest prime > n  (ensure prime table covers the
   required range; it grows automatically if necessary).  */
static idx_t next_prime(idx_t n)
{
    /* At least one more number beyond n must be sieved. */
    ensure_primes_atleast(n + 1);

    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_array[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }

    /* If no prime > n is found in the current table, extend the sieve. */
    while (lo == prime_count) {
        idx_t new_limit = current_limit * 2;
        if (new_limit <= n)
            new_limit = n + 1000;
        ensure_primes_atleast(new_limit);
        lo = 0; hi = prime_count;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (prime_array[mid] <= n)
                lo = mid + 1;
            else
                hi = mid;
        }
    }

    return prime_array[lo];
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;    /* value for n < offset is irrelevant */

    idx_t prev = prev_prime(n);
    idx_t next = next_prime(n);

    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t prod = (seq_t)prev * (seq_t)next;
    return sq - prod;
}
