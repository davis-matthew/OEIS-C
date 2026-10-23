/*
 * A048167  Integer quotients of numbers k divisible by the number of unitary
 *          divisors of k (A034444).
 * https://oeis.org/A048167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k(n) / 2^{ω(k(n))}, where k(n) is the n‑th integer k
 *            satisfying 2^{ω(k)} | k. Here ω(k) = number of distinct prime
 *            factors of k, i.e. the number of unitary divisors is 2^{ω(k)}.
 *            The function generates the sequence by scanning integers k in
 *            increasing order, testing the divisibility conditi...
 * Range:     u8 n <= 55, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Prime generation: we build a table of all primes up to a fixed limit.
   This limit is comfortably above the square root of any k that will
   appear for n up to about 10000. */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt   = 0;
static void init_primes(void)
{
    if (!prime_tbl) {
        /* 1,000,000 is more than enough for the target size. */
        size_t limit = 1000000;
        build_primes_upto(limit, &prime_tbl, &prime_cnt);
    }
}

/* Count distinct prime factors of n (ω(n)). */
static size_t distinct_prime_count(uint64_t n)
{
    size_t cnt = 0;
    uint64_t t = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * (uint64_t)prime_tbl[i] <= t; ++i) {
        uint64_t p = prime_tbl[i];
        if (t % p == 0) {
            ++cnt;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) ++cnt;   /* remaining prime factor > sqrt(n) */
    return cnt;
}

/* Dynamic array for memoised sequence values. */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static inline void grow_cache_to(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Current integer being examined for the next term.  Starts at 1. */
static idx_t current_k = 1;

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* 1‑based offset; n should be ≥ 1. */
    if (n == 0) return (seq_t)0;

    /* Initialise globals on first call. */
    if (!prime_tbl) init_primes();

    /* Return cached value if already computed. */
    if ((size_t)(n - 1) < cache_len) {
        return cache[n - 1];
    }

    /* Generate sequence terms until we reach the desired index. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        while (true) {
            uint64_t k = current_k;
            size_t omega = distinct_prime_count(k);
            uint64_t divisor = ((uint64_t)1) << omega;  /* 2^{ω(k)} */
            if (k % divisor == 0) {
                /* Valid k; compute quotient. */
                seq_t val = ((seq_t)k) / ((seq_t)divisor);
                grow_cache_to(cache_len + 1);
                cache[cache_len++] = val;
                ++current_k;                /* next integer to test */
                break;
            }
            ++current_k;
        }
    }
    return cache[n - 1];
}
