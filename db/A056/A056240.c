/*
 * A056240  Smallest number whose prime divisors (taken with multiplicity) add
 *          to n.
 * https://oeis.org/A056240
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 785, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: computes the smallest number whose prime divisors
   (with multiplicity) sum to n by recursion and memoization. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dependencies from utils */
#include "../utils/is_prime.h"            /* bool is_prime(idx_t n); */
#include "../utils/build_primes_upto.h"    /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
static void ensure_cache_size(idx_t n) {
    if (cache_vec.size() > (size_t)n) return;
    cache_vec.resize(n + 1);
    for (size_t i = 0; i < cache_vec.size(); ++i) cache_vec[i] = 0;
}
static seq_t get_cache(idx_t n)       { return cache_vec[n]; }
static void set_cache(idx_t n, seq_t v){ cache_vec[n] = v; }
#else
static seq_t *cache = NULL;
static size_t cache_size = 0;
static void ensure_cache_size(idx_t n) {
    if (cache_size > (size_t)n) return;
    size_t new_size = (size_t)n + 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (ptr == NULL) abort();
    cache = ptr;
    for (size_t i = 0; i < new_size; ++i) cache[i] = 0;
    cache_size = new_size;
}
static seq_t get_cache(idx_t n)       { return cache[n]; }
static void set_cache(idx_t n, seq_t v){ cache[n] = v; }
#endif

/* Prime list structure */
static uint64_t *primes = NULL;   /* array of primes up to max limit */
static size_t   prime_cnt = 0;    /* number of primes in 'primes' */
static idx_t    max_prime_limit = 0; /* largest number for which primes are generated */

static void ensure_primes(idx_t limit) {
    if (limit <= max_prime_limit) return;
    if (primes != NULL) free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    max_prime_limit = limit;
}

/* Recursive computation of a(n) with memoization */
static seq_t compute_a(idx_t n) {
    /* n >= 2 guaranteed by caller */
    seq_t cached = get_cache(n);
    if (cached != 0) return cached;

    /* Ensure primes up to n are available */
    ensure_primes(n);

    /* Base case: prime numbers */
    if (is_prime(n)) {
        set_cache(n, (seq_t)n);
        return (seq_t)n;
    }

    /* Recursive case: try all primes p <= n/2 */
    seq_t best = 0;  /* 0 means undefined */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n / 2) break;
        idx_t rem = n - p;
        seq_t prod = (seq_t)p * compute_a(rem);
        if (best == 0 || prod < best) best = prod;
    }

    set_cache(n, best);
    return best;
}

/* Public API: required function name and linkage */
seq_t a(idx_t n) {
    if (n < 2) return 0;  /* undefined for offset below 2 */
    ensure_cache_size(n);
    return compute_a(n);
}
