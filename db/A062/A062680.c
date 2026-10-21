/*
 * A062680  Composite numbers whose divisors (except 1) all contain the digit
 *          9.
 * https://oeis.org/A062680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Find composite numbers whose divisors (except 1) all contain the digit 9.
   The routine searches successive integers that contain the digit 9, skips primes,
   and checks the divisor condition by generating the factorization with factor_u64
   and testing every divisor derived from that factorization.  Results are cached
   for repeated calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <new>
#endif

#include "../utils/isPrimeGeneric.h"
#include "../utils/factor_u64.h"

/* --------------------------------------------------------------------- */
/* Helper: test if a value contains the digit 9.  Works for both 64‑bit
   integers and arbitrary‑precision seq_t integers, provided seq_t supports
   %, /, > operations with small integer literals. */
static bool has_digit9(seq_t value)
{
    seq_t v = value;
    while (v > (seq_t)0) {
        if ((v % (seq_t)10) == (seq_t)9)
            return true;
        v = v / (seq_t)10;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Cache storage for discovered sequence terms.  The cache is grown
   asymmetrically and is shared between all calls to a().  The cache
   is protected against overflow of its size_t count. */
static seq_t  *cache   = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;

/* Grow the cache to at least new_cap elements.  Supports both C and C++ builds. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    seq_t *tmp = NULL;

#ifdef __cplusplus
    try {
        tmp = new seq_t[new_cap];
    } catch (const std::bad_alloc &) {
        tmp = NULL;
    }
#else
    tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif

    if (!tmp) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache   = NULL;
        cache_cap = 0;
        return;
    }

#ifdef __cplusplus
    /* copy previously cached values */
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    delete[] cache;
#endif

    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Prime factorization storage for a candidate number.  The arrays are
   static to avoid repeated allocations in the recursive divisor test. */
static uint64_t _pr[64];
static uint64_t _ex[64];
static size_t   _cnt;

/* Recursive divisor check.
   cur holds the partial product of divisors generated so far. */
static bool _check_divs(size_t idx, seq_t cur)
{
    if (idx == _cnt) {
        if (cur > (seq_t)1 && !has_digit9(cur))
            return false;
        return true;
    }

    seq_t val = cur;
    for (uint64_t e = 0; e <= _ex[idx]; ++e) {
        if (!_check_divs(idx + 1, val))
            return false;
        if (e == _ex[idx])
            break;
        val = val * (seq_t)_pr[idx];
    }
    return true;
}

/* Test if candidate (uint64_t) satisfies the sequence property. */
static bool satisfies_candidate(uint64_t cand)
{
    if (isPrimeGeneric((seq_t)cand))
        return false;

    factor_u64(cand, _pr, _ex, &_cnt);
    return _check_divs(0, (seq_t)1);
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  idx_t n is 1‑based; a(1) is the first term.
   The function is memoized: subsequent calls can retrieve previously
   computed terms regardless of the order of n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for index 0 */

    /* Fast return if already cached */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Iterator for candidates that contain the digit 9.  Start from 1691,
       the first term of the sequence. */
    static uint64_t next_candidate = 1691;

    /* Expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Find next valid candidate */
        while (true) {
            if (!has_digit9(cand)) {
                ++cand;
                continue;
            }
            if (isPrimeGeneric((seq_t)cand)) {
                ++cand;
                continue;
            }
            if (satisfies_candidate(cand))
                break;   /* Valid term found */
            ++cand;
        }

        /* Grow cache if needed */
        if (cache_len == cache_cap) {
            grow_cache(cache_cap ? cache_cap * 2 : 16);
            /* Allocation failure yields zero cache_cap; in that rare case
               we simply abort future additions. */
            if (!cache_cap)
                break;
        }

        /* Store the term */
        cache[cache_len++] = (seq_t)cand;

        /* Set next search start */
        next_candidate = cand + 1;
    }

    return cache[(size_t)n - 1];
}
