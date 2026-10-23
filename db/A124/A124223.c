/*
 * A124223  Table T(n,k) = reciprocal of k modulo prime(n), for 1 <= k <
 *          prime(n), read by rows.
 * https://oeis.org/A124223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row,col) where row is the smallest r with *
 *            Σ_{i=1}^{r-1} (prime(i)-1) < n ≤ Σ_{i=1}^{r} (prime(i)-1), * col
 *            = n - Σ_{i=1}^{r-1} (prime(i)-1), * and T(r,c) = c^(prime(r)-2)
 *            mod prime(r)
 * Range:     u8 n <= 4181, u16 n <= 4181, u32 n <= 4181, u64 n <= 4181,
 *            u128 n <= 4181, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 16‑bit or 64‑bit arithmetic is used strictly per the rules. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions from the build.  Their headers must be included. */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x);          */
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base,
                                *              uint64_t exp,
                                *              seq_t mod);   */

/* Cache of primes generated so far. */
static uint64_t * _prime_cache   = NULL;   /* array of generated primes   */
static idx_t      _prime_len     = 0;      /* allocated array length      */
static idx_t      _prime_used    = 0;      /* number of primes stored     */
static idx_t      _cum_sum       = 0;      /* Σ (prime[i]-1) for i<used   */

/* Ensure that the cache can hold at least `needed` primes. */
static void _ensure_prime_cache(size_t needed)
{
    if (_prime_cache == NULL) {
        size_t init_len = needed > 0 ? needed : 2;
        _prime_cache = (uint64_t *)malloc(init_len * sizeof(uint64_t));
        _prime_len   = (idx_t)init_len;
    } else if (needed > _prime_len) {
        size_t new_len = _prime_len * 2;
        if (new_len < needed) new_len = needed;
        _prime_cache = (uint64_t *)realloc(_prime_cache, new_len * sizeof(uint64_t));
        _prime_len   = (idx_t)new_len;
    }
}

/* Generate consecutive primes until the cumulative sum of
 * (prime-1) exceeds or equals `target`.  The cache holds the primes
 * found up to that point. */
static void _generate_primes_to(idx_t target)
{
    static idx_t candidate = 1;  /* We start checking from 2. */
    while (_cum_sum < target) {
        ++candidate;             /* next candidate to test          */
        if (is_prime(candidate)) {
            _ensure_prime_cache(_prime_used + 1);
            _prime_cache[_prime_used++] = (uint64_t)candidate;
            _cum_sum += candidate - 1ULL;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1, return 0 */

    /* Ensure that we have enough primes to reach index n. */
    _generate_primes_to(n);

    /* Find the row containing the n‑th element. */
    idx_t cumulative_before = 0;
    for (idx_t idx = 0; idx < _prime_used; ++idx) {
        idx_t len = _prime_cache[idx] - 1ULL;           /* length of this row */
        idx_t cumulative_after = cumulative_before + len;
        if (n <= cumulative_after) {                   /* this is the row */
            idx_t k = n - cumulative_before;           /* 1 ≤ k < prime */
            uint64_t p = _prime_cache[idx];            /* modulus */
            return mod_pow((seq_t)k, p - 2ULL, (seq_t)p);
        }
        cumulative_before = cumulative_after;
    }
    /* Should never be reached if prime generation worked. */
    return 0;
}
