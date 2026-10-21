/*
 * A075237  Primes whose base 7 reversal is also prime.
 * https://oeis.org/A075237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 2225, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes whose base 7 reversal is also prime */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/rev_base7.h"

/* ----------------------------------------------------------------------
 *  Precomputation:  a fixed sieve up to LIMIT, then gather the wanted
 *  primes into an array.  The array is built once on the first call
 *  of a(idx_t).  LIMIT is chosen to accommodate more than 10 000 terms.
 *  -------------------------------------------------------------------- */

static const uint64_t LIMIT = 6000000ULL;          /* sieve bound */

static uint8_t *is_prime = NULL;                  /* prime flags */
static uint32_t *primes = NULL;                   /* list of primes up to LIMIT */
static size_t primes_cnt = 0;                     /* number of primes stored */

static seq_t *good = NULL;                        /* qualifying primes */
static size_t good_cnt = 0;                       /* number of qualifying primes */

/* Build a simple Eratosthenes sieve up to LIMIT and collect the primes. */
static void build_sieve(void) {
    is_prime = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime) return;                       /* allocation failure */
    memset(is_prime, 1, (LIMIT + 1) * sizeof(uint8_t));
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    primes_cnt = 0;
    /* Rough initial capacity: number of primes < LIMIT is < LIMIT/ln(LIMIT) */
    size_t cap = LIMIT / 10 + 10;
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_cnt >= cap) {
                cap *= 2;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
}

/* From the list of primes, keep those whose base‑7 reversal is also prime. */
static void build_good(void) {
    good_cnt = 0;
    size_t cap = LIMIT / 10;                      /* rough upper bound   */
    good = (seq_t *)malloc(cap * sizeof(seq_t));
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t r = rev_base7((uint64_t)p);
        if (r <= LIMIT && is_prime[(size_t)r]) {
            if (good_cnt >= cap) {
                cap *= 2;
                good = (seq_t *)realloc(good, cap * sizeof(seq_t));
            }
            good[good_cnt++] = (seq_t)p;
        }
    }
}

/* Ensure that the precomputation has been performed. */
static void init_if_needed(void) {
    static bool initialized = false;
    if (!initialized) {
        build_sieve();
        build_good();
        initialized = true;
    }
}

/* ----------------------------------------------------------------------
 *  Public function required by the OEIS test harness.
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* a(1) is the first term; the offset is 1. */
    if (n == 0) return (seq_t)0;                 /* unused, but defined */
    init_if_needed();
    if ((idx_t)good_cnt < n)                     /* insufficient data */
        return (seq_t)0;
    return good[n - 1];
}
