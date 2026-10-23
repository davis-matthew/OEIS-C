/*
 * A174185  Numbers k such that the k-th prime minus the k-th even nonprime is
 *          prime.
 * https://oeis.org/A174185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that prime(k) - (even nonprime)(k) is prime.
   Computed by generating primes up to a fixed limit and testing the
   condition for each k.  The result is memoised so that repeated calls
   for arbitrary n are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

#define SIEVE_LIMIT 10000000UL   /* enough to cover many terms (≈664k primes) */

static uint32_t *primes = NULL;          /* array of primes */
static size_t primes_cap = 0;            /* allocated capacity */
static size_t primes_cnt = 0;            /* number of primes stored */

static seq_t *terms = NULL;             /* memoised sequence values a(k) */
static size_t terms_cap = 0;             /* capacity of terms array */
static size_t terms_cnt = 0;             /* number of terms stored */

/* current index of k being processed (1‑based) */
static idx_t current_k = 1;

/* helper to double the capacity of an unsigned array */
static void ensure_prime_cap(void)
{
    size_t new_cap = (primes_cap == 0) ? 200000 : primes_cap * 2;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);                /* abort on OOM */
    primes = tmp;
    primes_cap = new_cap;
}

static void ensure_terms_cap(void)
{
    size_t new_cap = (terms_cap == 0) ? 32 : terms_cap * 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    terms = tmp;
    terms_cap = new_cap;
}

/* initialise the prime sieve once */
static void init_primes(void)
{
    bool *is_composite = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(1);

    /* approximate number of primes: x/logx */
    primes_cap = (size_t)(SIEVE_LIMIT / 14.0) + 100;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == primes_cap) ensure_prime_cap();
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    free(is_composite);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, but harmless */

    /* initialise on first call */
    static bool primed = false;
    if (!primed) {
        init_primes();
        primed = true;
    }

    /* generate terms until we have the nth one */
    while (terms_cnt < (size_t)n) {
        /* get k‑th prime, k = current_k (1‑based) */
        uint32_t p = primes[(size_t)(current_k - 1)];
        seq_t prime_val = (seq_t)p;
        seq_t e = (current_k == 1) ? (seq_t)0 : (seq_t)2 * (seq_t)current_k;
        bool ok = false;

        if (prime_val > e) {
            seq_t diff = prime_val - e;
            if (diff >= (seq_t)2 && is_prime_small(diff))
                ok = true;
        }

        if (ok) {
            if (terms_cnt == terms_cap) ensure_terms_cap();
            terms[terms_cnt++] = (seq_t)current_k;
        }
        ++current_k;
    }

    return terms[(size_t)(n - 1)];
}
