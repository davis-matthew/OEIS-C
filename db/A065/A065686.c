/*
 * A065686  Number of primes <= prime(n) which begin with a 7.
 * https://oeis.org/A065686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #{k ≤ n : the k‑th prime starts with digit 7}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   nth_prime_upper_bound, starts_with_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function lazily builds a sieve up to the n‑th prime and keeps a
   running prefix count of primes that start with 7.  Subsequent calls
   reuse the cached data. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/nth_prime_upper_bound.h"
#include "../utils/starts_with_7.h"

/* ------------------------------------------------------------------ */
/* Helper: upper bound for the n-th prime (n ≥ 6)                      */

/* ------------------------------------------------------------------ */
/* Helper: does prime p start with digit 7?                           */

/* ------------------------------------------------------------------ */
/* Global cache: primes and cumulative counts of those starting with 7 */
/* ------------------------------------------------------------------ */
static uint64_t *prime_cache    = NULL;
static seq_t   *prefix_cache    = NULL;
static size_t   prime_cache_cnt = 0;
static size_t   prime_cache_cap = 0;
static size_t   prefix_cache_cap = 0;

/* ------------------------------------------------------------------ */
/* Expand the cache to hold at least n primes.                         */
/* This routine may re‑compute the sieve from scratch if the required
   number of primes exceeds the current cache.                       */
/* ------------------------------------------------------------------ */
static void extend_cache_to(idx_t n)
{
    if (prime_cache_cnt >= (size_t)n) return;   /* already sufficient */

    uint64_t limit = nth_prime_upper_bound((uint64_t)n);

    /* ---- Sieve up to 'limit' ---------------------------------- */
    size_t sz = (size_t)limit + 1;
    char *is_composite = (char *)calloc(sz, 1);
    if (!is_composite) abort();

    size_t root = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= root; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }

    /* Count total primes up to 'limit' to know required capacity */
    size_t total_primes = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++total_primes;

    /* Grow prime_cache if necessary */
    if (prime_cache_cap < total_primes) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 1024;
        while (new_cap < total_primes) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }

    /* Grow prefix_cache if necessary */
    if (prefix_cache_cap < total_primes) {
        size_t new_cap = prefix_cache_cap ? prefix_cache_cap * 2 : 1024;
        while (new_cap < total_primes) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prefix_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prefix_cache = tmp;
        prefix_cache_cap = new_cap;
    }

    /* Populate prime and prefix arrays */
    seq_t acc = 0;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            prime_cache[idx] = i;
            if (starts_with_7(i)) acc += (seq_t)1;
            prefix_cache[idx] = acc;
            ++idx;
        }
    }
    prime_cache_cnt = idx;

    free(is_composite);
}

/* ================================================================== */
/* Public function required by the build system.                       */
/* ================================================================== */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    extend_cache_to(n);
    return prefix_cache[n - 1];   /* zero‑based array */
}
