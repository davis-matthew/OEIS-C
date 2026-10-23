/*
 * A161908  Array read by rows in which row n lists the divisors of n that are
 *          >= sqrt(n).
 * https://oeis.org/A161908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 740, u16 n <= 3550, u32 n <= 3550, u64 n <= 3550,
 *            u128 n <= 3550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS sequence A161908.
 * For each integer n ≥ 1, it outputs the n‑th superior divisor
 * in the flattened row‑major order of superior divisors of
 * all integers ≥ 1. Superior divisors of a number m are those
 * d that satisfy d ≥ m / d.  They are produced for each row
 * m in ascending order.
 *
 * The implementation stores already computed terms in a cache
 * and extends it lazily until the requested index n is reached.
 * This is O(n) in time and space for a single call, and works
 * for any n ≥ 1.  No external libraries are required beyond
 * the standard headers.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  helper: floor integer square root for idx_t (uint64_t) ---------- */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid */
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* ----------  cache for computed terms ---------- */
static size_t _cache_len   = 0;            /* number of cached terms */
static size_t _cache_cap   = 0;            /* allocated capacity   */
static seq_t  *_cache       = NULL;       /* cache array          */
static idx_t   _processed   = 0;           /* last number for which row has been filled */

/* ----------  append a value to the cache ---------- */
static void _push(seq_t v)
{
    if (_cache_len >= _cache_cap) {
        size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                      /* out of memory */
        _cache = tmp;
        _cache_cap = new_cap;
    }
    _cache[_cache_len++] = v;
}

/* ----------  main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0 */

    /* extend cache until it holds at least n terms */
    while (_cache_len < (size_t)n) {
        ++_processed;
        idx_t row   = _processed;
        idx_t sqrtv = _floor_isqrt_u64(row);

        /* enumerate d ≥ sqrt(row) by checking each divisor i ≤ sqrt(row) */
        for (idx_t i = sqrtv; i != 0; --i) {
            if (row % i == 0) {
                seq_t val = (seq_t)(row / i);    /* convert to seq_t */
                _push(val);
            }
        }
    }
    return _cache[n - 1];
}
