/*
 * A137451  a(1)=1. a(n) = the smallest integer > a(n-1) such that a(n)+a(k),
 *          for some k where 1<=k<=n, is the n-th prime.
 * https://oeis.org/A137451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer > a(n‑1) such that * a(n)+a(k)=p_n
 *            for some 1≤k≤n, * where p_n is the n‑th prime. * * The algorithm
 *            keeps a cache of previously computed a(i) * and a list of the
 *            first primes. For each new n it * scans all previously known
 *            a(k) (k < n) and selects the * smallest candid...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/build_primes_upto.h"

static seq_t * _cache   = NULL;      /* cache[1..n] of a(n) */
static size_t _cache_cap = 0;        /* allocated capacity for _cache */
static idx_t   _last_computed = 0;   /* largest n already computed */

static uint64_t * _primes   = NULL;  /* list of primes (uint64_t) */
static size_t    _prime_cnt = 0;     /* number of primes stored */

/* Ensure _cache can hold indices up to n (index 1 is used). */
static void _ensure_cache(idx_t n)
{
    if (_cache_cap >= n + 1) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    _cache   = new_cache;
    _cache_cap = new_cap;
}

/* Ensure we have at least n primes in _primes. */
static void _ensure_primes(idx_t n)
{
    if (_prime_cnt >= (size_t)n) return;

    /* Rough upper bound for the n-th prime (valid for n≥6). */
    idx_t limit = (n < 6) ? 15 : n * 20 + 100;

    /* Build primes up to the estimated limit. */
    if (_primes) free(_primes);
    build_primes_upto(limit, &_primes, &_prime_cnt);

    /* Double the limit if we haven't gathered enough primes. */
    while (_prime_cnt < (size_t)n) {
        limit *= 2;
        free(_primes);
        build_primes_upto(limit, &_primes, &_prime_cnt);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1 */

    _ensure_cache(n);
    _ensure_primes(n);

    /* Initialise a(1). */
    if (_last_computed == 0) {
        _cache[1] = (seq_t)1;
        _last_computed = 1;
    }

    for (idx_t i = _last_computed + 1; i <= n; ++i) {
        seq_t prev = (i == 1) ? 0 : _cache[i - 1];
        uint64_t prime = _primes[i - 1];       /* i-th prime (0‑based) */

        uint64_t best = UINT64_MAX;

        /* Check all k < i (known a(k)). */
        for (idx_t k = 1; k < i; ++k) {
            seq_t ak = _cache[k];
            if ((uint64_t)ak > prime) continue;            /* candidate would be negative */

            uint64_t cand = prime - (uint64_t)ak;
            if (cand <= (uint64_t)prev) continue;          /* must be strictly greater than a(i‑1) */

            if (cand < best) best = cand;
        }

        /* Special case for i == 1 (prime = 2). */
        if (i == 1) {
            if (best == UINT64_MAX || 1 < best) best = 1;
        }

        _cache[i] = (seq_t)best;
    }

    _last_computed = n;
    return _cache[n];
}
