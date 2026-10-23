/*
 * A025930  Expansion of 1/((1-2*x)*(1-3*x)*(1-5*x)*(1-6*x)).
 * https://oeis.org/A025930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 16*a(n-1) - 91*a(n-2) + 216*a(n-3) - 180*a(n-4)
   initial values: a(0)=1, a(1)=16, a(2)=165, a(3)=1400 */
#include <stdlib.h>

static seq_t * _cache = NULL;
static idx_t   _cache_len = 0;
static idx_t   _cache_cap = 0;

/* Ensure that the cache contains terms up to index n (inclusive) */
static void _ensure_cache(idx_t n)
{
    /* allocate initial cache if needed */
    if (_cache == NULL) {
        _cache_cap = 4;
        _cache = (seq_t *)malloc(sizeof(seq_t) * _cache_cap);
        if (_cache == NULL) abort();  /* cannot happen in normal use */
        _cache[0] = (seq_t)1;
        _cache[1] = (seq_t)16;
        _cache[2] = (seq_t)165;
        _cache[3] = (seq_t)1400;
        _cache_len = 4;
    }

    /* grow the allocated array if necessary */
    if (n >= _cache_cap) {
        idx_t new_cap = _cache_cap > 0 ? _cache_cap : 4;
        while (new_cap <= n) new_cap <<= 1;
        _cache = (seq_t *)realloc(_cache, sizeof(seq_t) * new_cap);
        if (_cache == NULL) abort();
        _cache_cap = new_cap;
    }

    /* compute missing terms */
    if (n >= _cache_len) {
        for (idx_t i = _cache_len; i <= n; ++i) {
            if (i < 4) continue;           /* base values already set */
            seq_t term = 16 * _cache[(size_t)(i - 1)];
            term -= 91 * _cache[(size_t)(i - 2)];
            term += 216 * _cache[(size_t)(i - 3)];
            term -= 180 * _cache[(size_t)(i - 4)];
            _cache[(size_t)i] = term;
        }
        _cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    _ensure_cache(n);
    return _cache[(size_t)n];
}
