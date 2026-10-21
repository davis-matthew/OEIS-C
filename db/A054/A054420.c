/*
 * A054420  Number of connectable 3 X n binary matrices.
 * https://oeis.org/A054420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 3*a(n-2) + 5*a(n-3) Initial values: a(1)=1,
 *            a(2)=3, a(3)=13 The sequence is defined for n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static size_t _cache_len = 0;        /* number of terms already stored (a(1)...a(_cache_len)) */
static size_t _cache_cap = 0;        /* allocated capacity for the cache array */
static seq_t *_cache = NULL;         /* 0‑based array: _cache[0] = a(1) */

/* Ensure that at least n terms (1‑indexed) are cached */
static void _ensure_cache(idx_t n)
{
    if (n == (idx_t)_cache_len || n == 0)
        return;

    /* Resize capacity if necessary */
    size_t needed = (size_t)n;
    if (_cache_cap < needed) {
        size_t new_cap = _cache_cap ? _cache_cap * 2 : 4;
        if (new_cap < needed)
            new_cap = needed;
        _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        _cache_cap = new_cap;
    }

    /* Compute new terms */
    for (size_t i = _cache_len; i < (size_t)n; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)1;          /* a(1) */
        else if (i == 1)     val = (seq_t)3;          /* a(2) */
        else if (i == 2)     val = (seq_t)13;         /* a(3) */
        else                 val = 7 * _cache[i-1]
                               - 3 * _cache[i-2]
                               + 5 * _cache[i-3];
        _cache[i] = val;
    }
    _cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1; return 0 as a safe value */
    _ensure_cache(n);
    return _cache[(size_t)n - 1];
}
