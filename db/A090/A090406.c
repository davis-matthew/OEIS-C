/*
 * A090406  a(n) = PrimePi(n+3) - PrimePi(n).
 * https://oeis.org/A090406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi(n+3) - PrimePi(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* prime‑count table up to the current limit
   pi_arr[i] = π(i) for i = 0 .. pi_limit */
static size_t pi_limit = 0;
static uint32_t *pi_arr = NULL;

/* list of primes found so far */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static uint64_t *primes = NULL;

/* extend the prime list and π table up to new_limit (inclusive) */
static void extend_pi(size_t new_limit)
{
    if (new_limit <= pi_limit) return;

    /* resize π array */
    uint32_t *tmp_pi = (uint32_t *)realloc(pi_arr,
                                           (new_limit + 1) * sizeof(*tmp_pi));
    if (!tmp_pi) abort();
    pi_arr = tmp_pi;

    /* ensure enough space for primes storage */
    if (primes_cap < primes_cnt * 2 + 16) {
        primes_cap = primes_cnt * 2 + 16;
        uint64_t *tmp_p = (uint64_t *)realloc(primes,
                                               primes_cap * sizeof(*tmp_p));
        if (!tmp_p) abort();
        primes = tmp_p;
    }

    /* initialise new entries of π (use previous value as base) */
    if (pi_limit == 0) {
        /* First time: set π(0) = 0 and π(1) = 0.  |π_arr| >= 2 because
           new_limit >= 4 for all calls. */
        pi_arr[0] = 0;
        pi_arr[1] = 0;
    }

    for (size_t n = pi_limit + 1; n <= new_limit; ++n) {
        bool is_prime = true;
        if (n < 2) {
            is_prime = false;
        } else {
            size_t limit = (size_t)(sqrt((double)n) + 0.5);
            for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
                if (n % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) { /* unlikely with these sizes */
                primes_cap *= 2;
                uint64_t *tmp_p = (uint64_t *)realloc(primes,
                                                       primes_cap * sizeof(*tmp_p));
                if (!tmp_p) abort();
                primes = tmp_p;
            }
            primes[primes_cnt++] = n;
        }
        pi_arr[n] = pi_arr[n - 1] + (is_prime ? 1 : 0);
    }
    pi_limit = new_limit;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* we need π(n) and π(n+3) */
    extend_pi(n + 3);
    return (seq_t)(pi_arr[n + 3] - pi_arr[n]);
}
