/*
 * A212165  Numbers k such that the maximum exponent in its prime
 *          factorization is not less than the number of positive exponents
 *          (A051903(k) >= A001221(k)).
 * https://oeis.org/A212165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 4900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212165
   a(n) = the nth positive integer k such that the maximum exponent in
   the prime factorization of k is not less than the number of
   distinct prime factors of k.  It is the sorted list of all integers
   satisfying max(exp_i) ≥ distinct_primes(k).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/floor_sqrt.h"          /* floor_sqrt */

typedef uint64_t idx_t;       /* interfaces guarantee idx_t is uint64_t */

/* Helper: ensure we have all primes ≤ limit in prime_list. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static uint64_t prime_sieve_limit = 0;

static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_sieve_limit) return;
    free(prime_list);                    /* free old sieve if any */
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_sieve_limit = limit;
}

/* Test whether n satisfies the definition of A212165. */
static bool satisfies(uint64_t n)
{
    if (n == 1) return true;  /* 1 is included by convention */

    uint64_t temp = n;
    size_t distinct = 0;
    unsigned int max_exp = 0;

    /* factor using the precomputed prime list. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            ++distinct;
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > max_exp) max_exp = e;
        }
    }

    /* Remaining factor > 1 is prime. */
    if (temp > 1) {
        ++distinct;
        if (max_exp < 1) max_exp = 1;
    }

    return (max_exp >= distinct);
}

/* Main sequence routine. */
static seq_t *seq = NULL;  /* cache of found terms (uint64_t values) */
static size_t seq_len = 0;
static uint64_t candidate = 1;  /* next integer to test */

seq_t a(idx_t n)
{
    /* Lazy generate enough terms on demand. */
    while (seq_len < n) {
        /* Ensure we have primes up to sqrt(candidate). */
        uint64_t limit = (uint64_t)floor_sqrt((seq_t)candidate);
        if (limit < 2) limit = 2;
        ensure_primes_upto(limit);

        /* Find next integer satisfying the property. */
        while (!satisfies(candidate)) ++candidate;

        /* Store it. */
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * (seq_len + 1));
        seq[seq_len] = (seq_t)candidate;
        ++seq_len;
        ++candidate;
    }

    return seq[n - 1];
}
