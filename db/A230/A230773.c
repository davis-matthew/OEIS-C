/*
 * A230773  Minimum number of steps in an alternate definition of the Sieve of
 *          Eratosthenes needed to identify n as prime or composite.
 * https://oeis.org/A230773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_sqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) for OEIS A230773.
   For n ≤ 2    → 0
   For n = 3    → 1
   For composite n → index of its smallest prime factor
   For prime n > 3 → index of the largest prime < sqrt(n)
   (indices are 1‑based; prime 2 has index 1, prime 3 has index 2, …)  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_floor_sqrt_u64.h"

static uint64_t *primes = NULL;   /* global prime table */
static size_t   primes_cnt = 0;   /* number of primes stored */
static idx_t    primes_limit = 0; /* largest number sieved */

static void ensure_primes_upto(idx_t limit) {
    if (limit <= primes_limit)
        return;
    if (primes)
        free(primes);
    /* simple sieve up to limit (inclusive) */
    unsigned char *mark = (unsigned char*)calloc((size_t)limit + 1, 1);
    if (!mark)
        exit(1); /* allocation failure */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (mark[p])
            continue;
        for (idx_t j = p * p; j <= limit; j += p)
            mark[j] = 1;
    }
    size_t sz = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!mark[i])
            ++sz;
    primes = (uint64_t*)malloc(sz * sizeof(uint64_t));
    if (!primes)
        exit(1);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!mark[i])
            primes[pos++] = (uint64_t)i;
    primes_cnt = sz;
    primes_limit = limit;
    free(mark);
}

static bool _is_prime_u64(idx_t n) {
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;
    idx_t lim = _floor_sqrt_u64(n);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((__uint128_t)p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

static /* 1‑based */ size_t _prime_index_uint64(uint64_t p) {
    /* binary search for p in primes (if primes contain p) */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < primes_cnt && primes[lo] == p)
        return lo + 1;      /* 1‑based index */
    return 0;              /* should not happen */
}

static uint64_t _smallest_prime_factor_u64(idx_t n) {
    idx_t lim = _floor_sqrt_u64(n);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((__uint128_t)p * p > n)
            break;
        if (n % p == 0)
            return p;
    }
    return n; /* n is prime */
}

static uint64_t _largest_prime_leq_u64(idx_t n) {
    /* assume primes array is up to at least n */
    size_t idx = 0;
    while (idx < primes_cnt && primes[idx] <= n)
        ++idx;
    if (idx == 0)
        return 0;
    return primes[idx - 1];
}

seq_t a(idx_t n) {
    if (n <= 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)1;

    idx_t sq = _floor_sqrt_u64(n);
    if (sq < 2)
        sq = 2;
    ensure_primes_upto(sq);

    if (_is_prime_u64(n)) {          /* prime > 3 */
        uint64_t lp = _largest_prime_leq_u64(sq);
        /* lp will be at least 2, and for n=5, lp=2 */
        size_t idx = _prime_index_uint64(lp);
        return (seq_t)idx;
    } else {                         /* composite */
        uint64_t spf = _smallest_prime_factor_u64(n);
        size_t idx = _prime_index_uint64(spf);
        return (seq_t)idx;
    }
}
