/*
 * A029067  Expansion of 1/((1-x)*(1-x^4)*(1-x^5)*(1-x^10)).
 * https://oeis.org/A029067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 4, 5, 10
 * Range:     u8 n <= 57, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const idx_t _parts[4] = { 1, 4, 5, 10 };

static seq_t *_dp        = NULL;          /* cached DP values a(0)…a(_cache_size-1) */
static size_t _cache_size = 0;           /* number of computed values      */
static size_t _cache_cap  = 0;           /* allocated array capacity       */

/* Ensure that _dp contains a(0)…a(n).  Recompute the array from scratch
   up to n whenever the cached maximum changes. */
static void
ensure_cache(idx_t n)
{
    size_t target = (size_t)n + 1;            /* number of values to compute */

    /* Grow the cache if necessary. */
    if (target > _cache_cap) {
        size_t new_cap = _cache_cap ? _cache_cap : 64;
        while (new_cap < target) new_cap <<= 1;
        seq_t *new_dp = (seq_t *)realloc(_dp, new_cap * sizeof(seq_t));
        if (!new_dp) return;                  /* allocation failure: leave cache as is */
        _dp        = new_dp;
        _cache_cap = new_cap;
    }

    /* Recompute the DP values from 0 up to n. */
    _dp[0] = (seq_t)1;
    for (size_t i = 1; i < target; ++i) _dp[i] = (seq_t)0;

    for (size_t k = 0; k < sizeof(_parts)/sizeof(_parts[0]); ++k) {
        idx_t part = _parts[k];
        if ((size_t)part >= target) continue;          /* part too large */
        for (size_t i = (size_t)part; i < target; ++i) {
            _dp[i] += _dp[i - (size_t)part];
        }
    }

    _cache_size = target;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return (size_t)n < _cache_size ? _dp[(size_t)n] : (seq_t)0;
}
