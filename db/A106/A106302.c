/*
 * A106302  Period of the Fibonacci 3-step sequence A000073 mod prime(n).
 * https://oeis.org/A106302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   period_of_tribonacci_mod_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of the Tribonacci sequence mod the n‑th prime (A106302) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/period_of_tribonacci_mod_prime.h"

/* ---------- utility: generate the first ~20 000 primes (up to 200 000) ---------- */
#define PRIME_LIMIT   200000          /* > the 10 000th prime (104 729) */
#define MAX_PRIMES    20000          /* number of primes < PRIME_LIMIT */

/* Static storage for primes */
static uint64_t primes[MAX_PRIMES];
static idx_t prime_count = 0;
static bool primes_initialized = false;

/* Sieve of Eratosthenes to fill primes[] */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* allocate a small boolean array for composites */
    unsigned char *comp = (unsigned char *)calloc(PRIME_LIMIT + 1, 1);
    if (!comp) return;       /* allocation failed – should not happen */

    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                comp[j] = 1;
        }
    }

    for (idx_t i = 2; i <= PRIME_LIMIT && prime_count < MAX_PRIMES; ++i) {
        if (!comp[i])
            primes[prime_count++] = i;
    }
    free(comp);
    primes_initialized = true;
}

/* Return the n‑th prime (1‑based) */
static inline uint64_t nth_prime(idx_t n)
{
    if (!primes_initialized) init_primes();
    return primes[n - 1];
}

/* ---------- memoized computation of A106302 ---------- */
static seq_t period_cache[20001];        /* 0 means “not computed” */
static bool cache_initialized = false;

/* -------------------------------------------------
   seq_t a(idx_t n)
   Return the period of the Tribonacci sequence modulo the n‑th prime.
   ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* allow any order of calls; cache per index */
    if (!cache_initialized) {
        memset(period_cache, 0, sizeof(period_cache));
        cache_initialized = true;
    }

    /* if already computed, return the cached value */
    if (period_cache[n])
        return period_cache[n];

    /* otherwise compute it */
    uint64_t p = nth_prime(n);
    period_cache[n] = period_of_tribonacci_mod_prime(p);
    return period_cache[n];
}
