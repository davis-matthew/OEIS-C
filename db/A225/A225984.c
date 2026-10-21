/*
 * A225984  Linear recurrence sequence with infrequent pseudoprimes, a(n) =
 *          -a(n-1) + a(n-2) - a(n-3) + a(n-5), with initial terms (5, -1, 3,
 *          -7, 11).
 * https://oeis.org/A225984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-2) - a(n-3) + a(n-5); a(0) = 5, a(1) = -1,
 *            a(2) = 3, a(3) = -7, a(4) = 11
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 37, i64 n <= 75, i128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[5] = {5, -1, 3, -7, 11};
static seq_t * _cache = NULL;
static idx_t   _cache_size   = 0;
static idx_t   _last_computed = 4;   /* highest index already stored */

/* Ensure that _cache contains values up to n (inclusive). */
static void _ensure_cache(idx_t n)
{
    if (n <= _last_computed) return;                     /* already computed */

    idx_t need_size = n + 1;
    if (_cache_size < need_size) {                      /* grow the buffer */
        seq_t *tmp = (seq_t *)realloc(_cache, need_size * sizeof(seq_t));
        _cache = tmp;
        _cache_size = need_size;
    }

    /* If this is the first allocation, fill the initial terms. */
    if (_cache_size == need_size && _last_computed == 4) {
        for (idx_t i = 0; i < 5; ++i) _cache[i] = _init[i];
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = _last_computed + 1; i <= n; ++i) {
        _cache[i] = -_cache[i-1] + _cache[i-2] - _cache[i-3] + _cache[i-5];
    }

    _last_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 5) return _init[n];
    _ensure_cache(n);
    return _cache[n];
}
