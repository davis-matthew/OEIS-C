/*
 * A187170  Number of 9-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n>11. Initial
 *            values: a(1)=0, a(2)=0, a(3)=167016, a(4)=2013360, a(5)=8227752,
 *            a(6)=21274896, a(7)=43422072, a(8)=76964016, a(9)=124223214,
 *            a(10)=187527168, a(11)=269203674
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 24, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init_a[11] = {
    (seq_t)0,          // a(1)
    (seq_t)0,          // a(2)
    (seq_t)167016,     // a(3)
    (seq_t)2013360,    // a(4)
    (seq_t)8227752,    // a(5)
    (seq_t)21274896,   // a(6)
    (seq_t)43422072,   // a(7)
    (seq_t)76964016,   // a(8)
    (seq_t)124223214,  // a(9)
    (seq_t)187527168,  // a(10)
    (seq_t)269203674   // a(11)
};

static seq_t * _cache = NULL;      /* cached values a(1)...a(n) */
static size_t  _cache_cap = 0;     /* allocated capacity of _cache */
static size_t  _cache_len = 0;     /* number of cached values */

/* Grow the cache to accommodate at least 'need' elements. */
static void _ensure_cache(size_t need)
{
    if (need <= _cache_cap) return;
    size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    _cache_cap = new_cap;
}

/* Compute a(n) using the recurrence and cache results. */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0; /* outside offset, undefined but return 0 */

    /* Return precomputed initial values directly. */
    if (n <= 11) return _init_a[n-1];

    /* Ensure cache holds values up to at least n. */
    if (_cache_len == 0) {
        /* Initialise cache with the first 11 terms. */
        _ensure_cache(11);
        for (idx_t i = 0; i < 11; ++i) _cache[i] = _init_a[i];
        _cache_len = 11;
    }

    if ((size_t)n > _cache_len) {
        _ensure_cache((size_t)n);
        for (idx_t k = (idx_t)_cache_len + 1; k <= n; ++k) {
            size_t i = (size_t)k - 1;               /* array index for a(k) */
            seq_t v1 = _cache[i-1];                 /* a(k-1) */
            seq_t v2 = _cache[i-2];                 /* a(k-2) */
            seq_t v3 = _cache[i-3];                 /* a(k-3) */
            seq_t v4 = _cache[i-4];                 /* a(k-4) */
            _cache[i] = (seq_t)(4 * v1 - 6 * v2 + 4 * v3 - v4);
        }
        _cache_len = (size_t)n;
    }

    return _cache[(size_t)n - 1];
}
