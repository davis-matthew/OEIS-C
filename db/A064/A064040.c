/*
 * A064040  Integers whose number of distinct prime divisors is prime.
 * https://oeis.org/A064040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of integers whose number of distinct prime
 * divisors is prime.  Computed by sieving prime factor counts. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* External link function required by the build system */
static seq_t *terms = NULL;          /* cache of computed sequence terms   */
static size_t  terms_len  = 0;        /* number of currently cached terms   */
static size_t  terms_cap  = 0;        /* capacity of the cache array         */

static uint8_t *omega = NULL;        /* omega[i] = number of distinct prime factors of i */
static size_t  sieve_limit = 0;      /* maximum index already sieved               */

static uint64_t next_candidate = 2;  /* next integer to test for inclusion       */

/* Extend the sieve upwards to at least @min_limit, recomputing
 * the omega array from scratch.  The new array has size min_limit+1.
 * This is O(min_limit log log min_limit) which is fast enough
 * for all reasonable n (≤ 10 000). */
static void extend_sieve(size_t min_limit)
{
    if (min_limit <= sieve_limit)
        return;

    size_t new_limit = min_limit;
    uint8_t *new_omega = (uint8_t *)malloc(new_limit + 1);
    if (!new_omega)
        exit(EXIT_FAILURE);   /* insufficient memory */

    memset(new_omega, 0, new_limit + 1);

    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_omega[i] == 0) {              /* i is prime */
            for (size_t j = i; j <= new_limit; j += i)
                new_omega[j] += 1;
        }
    }

    free(omega);
    omega = new_omega;
    sieve_limit = new_limit;
}

/* Ensure that the first @n terms of the sequence are computed and
 * stored in the static cache.  The caller requests a value possibly
 * far beyond what has been computed previously, so we loop until
 * enough terms are found. */
static void ensure_terms(idx_t n)
{
    while (terms_len < (size_t)n) {

        /* If the candidate is beyond our current sieve, grow it. */
        if ((uint64_t)next_candidate > sieve_limit)
            extend_sieve(next_candidate * 2);

        /* Count the distinct prime factors of the candidate. */
        uint8_t cnt = omega[(size_t)next_candidate];

        /* If that count is prime, store the candidate. */
        if (is_prime_small((seq_t)cnt)) {
            if (terms_len >= terms_cap) {
                size_t newcap = terms_cap ? terms_cap * 2 : 16;
                seq_t *newarr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!newarr)
                    exit(EXIT_FAILURE);
                terms = newarr;
                terms_cap = newcap;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }
}

/* Public sequence function as required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; 0 is not defined but harmless */
        return 0;

    ensure_terms(n);
    return terms[(size_t)n - 1];
}
