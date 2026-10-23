/*
 * A162237  Primes p such that p^2+3*p+1 is not prime.
 * https://oeis.org/A162237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162237
   a(n) = the n-th prime p such that p^2 + 3*p + 1 is not prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* storage for all primes that have been discovered so far */
static uint64_t *primes = NULL;
static size_t    primes_len = 0;
static size_t    primes_cap = 0;

/* storage for the primes that satisfy the condition */
static uint64_t *match_primes = NULL;
static size_t    match_len    = 0;
static size_t    match_cap    = 0;

/* the next integer to test for primality */
static uint64_t next_candidate = 2;

/* grow an array of uint64_t when needed */
static void ensure_primes_cap(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    primes = tmp;
    primes_cap = new_cap;
}

static void ensure_match_cap(size_t needed)
{
    if (needed <= match_cap) return;
    size_t new_cap = match_cap ? match_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(match_primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(1);
    match_primes = tmp;
    match_cap = new_cap;
}

/* test if x is prime using the already known primes (trial division) */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;          /* p*p would overflow or > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for 0 */

    /* one-time initialization */
    static bool initialized = false;
    if (!initialized) {
        primes = (uint64_t *)malloc(128 * sizeof(uint64_t));
        if (!primes) exit(1);
        primes_cap = 128;
        match_primes = (uint64_t *)malloc(128 * sizeof(uint64_t));
        if (!match_primes) exit(1);
        match_cap = 128;
        initialized = true;
    }

    /* generate primes until we have found enough matches */
    while (match_len < (size_t)n) {
        /* find the next prime */
        uint64_t cand;
        for (;;) {
            /* skip even numbers > 2 */
            if (next_candidate > 2 && (next_candidate & 1) == 0) {
                ++next_candidate;
                continue;
            }
            cand = next_candidate;
            ++next_candidate;          /* next integer for subsequent iterations */

            if (is_prime_candidate(cand)) {
                /* store the new prime */
                ensure_primes_cap(primes_len + 1);
                primes[primes_len++] = cand;

                /* test the expression */
                seq_t expr = (seq_t)cand * (seq_t)cand + (seq_t)3 * (seq_t)cand + 1;
                if (!is_prime_big(expr)) {
                    ensure_match_cap(match_len + 1);
                    match_primes[match_len++] = cand;
                }
                break;                  /* go on to the next prime */
            }
        }
    }

    /* return the n-th qualifying prime, converted to seq_t */
    return (seq_t)match_primes[n - 1];
}
