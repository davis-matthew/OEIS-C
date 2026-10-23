/*
 * A041417  Denominators of continued fraction convergents to sqrt(223).
 * https://oeis.org/A041417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 448*a(n-4) - a(n-8) Initial values: a(0)=1, a(1)=1,
 *            a(2)=14, a(3)=15, a(4)=434, a(5)=449, a(6)=6271, a(7)=6720
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t _init[8] = {
    1, 1, 14, 15, 434, 449, 6271, 6720
};

static seq_t * _cache = NULL;      /* memoisation table */
static size_t  _cache_size = 0;    /* number of initialized entries */
static size_t  _cache_cap  = 0;    /* allocated capacity */

/* Ensure that the cache has room for indices up to n inclusively. */
static void _ensure_cache(idx_t n) {
    if (_cache == NULL) {
        /* initial allocation: at least 16 entries */
        _cache_cap = 16;
        _cache = (seq_t*)malloc(_cache_cap * sizeof(seq_t));
        /* initialise with zeros */
        for (size_t i = 0; i < _cache_cap; ++i)
            _cache[i] = 0;
    }
    while (_cache_cap <= (size_t)n) {
        size_t new_cap = _cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            abort();
        }
        _cache = tmp;
        /* initialise the newly added slots to zero */
        for (size_t i = _cache_cap; i < new_cap; ++i)
            _cache[i] = 0;
        _cache_cap = new_cap;
    }
}

/* Compute term a(n) using memoisation. */
seq_t a(idx_t n) {
    /* Fast return for the first few terms */
    if (n < 8)
        return _init[(size_t)n];

    /* Ensure the cache exists and is large enough */
    _ensure_cache(n);

    /* If value already computed, return it */
    if (n < _cache_size)
        return _cache[(size_t)n];

    /* Compute intermediate terms up to n */
    /* Make sure earlier terms are stored for recurrence */
    for (size_t i = _cache_size; i <= (size_t)n; ++i) {
        if (i < 8) {
            _cache[i] = _init[i];
        } else {
            /* a(i) = 448*a(i-4) - a(i-8) */
            seq_t v1 = _cache[i - 4];
            seq_t v2 = _cache[i - 8];
            _cache[i] = 448 * v1 - v2;
        }
    }
    _cache_size = (size_t)n + 1;
    return _cache[(size_t)n];
}
