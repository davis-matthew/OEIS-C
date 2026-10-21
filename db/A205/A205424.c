/*
 * A205424  Number of (n+1) X 2 0..3 arrays with the number of equal 2 X 2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A205424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 73*a(n-2) + 39*a(n-3) - 384*a(n-4) - 432*a(n-5)
 *            for n>6 initial terms: a(1)=15, a(2)=102, a(3)=870, a(4)=7566,
 *            a(5)=68342, a(6)=609086
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_terms[6] = {
    (seq_t)15, (seq_t)102, (seq_t)870,
    (seq_t)7566, (seq_t)68342, (seq_t)609086
};

static seq_t * _cache = NULL;
static size_t  _cache_len = 0;
static size_t  _cache_cap = 0;

seq_t a(idx_t n)
{
    /* guard against illegal indices – behavior undefined if n<1 */
    if (n < 1) return (seq_t)0;

    /* initialise cache on first call */
    if (_cache == NULL) {
        _cache_cap = 6;
        if ((size_t)n > _cache_cap) _cache_cap = (size_t)n;
        _cache = (seq_t *)malloc(_cache_cap * sizeof(seq_t));
        if (!_cache) abort();
        for (size_t i = 0; i < 6; ++i) _cache[i] = _init_terms[i];
        _cache_len = 6;
    }

    /* grow cache if needed */
    if ((size_t)n > _cache_cap) {
        seq_t *tmp = (seq_t *)realloc(_cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();
        _cache = tmp;
        _cache_cap = (size_t)n;
    }

    /* compute missing terms */
    while (_cache_len < (size_t)n) {
        /* indices for recurrence are zero‑based */
        size_t i = _cache_len;                         /* term number i+1 */
        seq_t t = _cache[i-1]
                +   (seq_t)73 * _cache[i-2]
                +   (seq_t)39 * _cache[i-3]
                -   (seq_t)384 * _cache[i-4]
                -   (seq_t)432 * _cache[i-5];
        _cache[_cache_len++] = t;
    }

    return _cache[(size_t)n - 1];
}
