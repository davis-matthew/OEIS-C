/*
 * A041857  Denominators of continued fraction convergents to sqrt(450).
 * https://oeis.org/A041857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 39202*a(n-8) - a(n-16) (for n > 15) initial values: a(0)
 *            = 1, a(1) = 4, a(2) = 5, a(3) = 14, a(4) = 61, a(5) = 136, a(6)
 *            = 197, a(7) = 924, a(8) = 39005, a(9) = 156944, a(10) = 195949,
 *            a(11) = 548842, a(12) = 2391317, a(13) = 5331476, a(14) =
 *            7722793, a(15) = 36222648
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Pre‑computed first 16 terms – needed to seed the recurrence */
static const seq_t _base[16] = {
    (seq_t)1,   (seq_t)4,   (seq_t)5,   (seq_t)14,  (seq_t)61,
    (seq_t)136, (seq_t)197, (seq_t)924, (seq_t)39005,(seq_t)156944,
    (seq_t)195949,(seq_t)548842,(seq_t)2391317,(seq_t)5331476,
    (seq_t)7722793,(seq_t)36222648
};

/* Runtime cache of computed values */
static seq_t * _cache   = NULL;   /* pointer to the cache */
static idx_t  _cap      = 0;      /* allocated capacity (number of slots) */
static idx_t  _last     = 15;     /* highest index that has been computed */

/* Ensure that _cache can hold indices 0 … n */
static void _ensure_cache(idx_t n)
{
    if (_cache == NULL) {                /* first allocation */
        _cap = (n >= 16) ? n + 1 : 16;
        _cache = (seq_t*)calloc((size_t)_cap, sizeof(seq_t));
        if (_cache == NULL) return;      /* allocation failed – abort silently */
        for (idx_t i = 0; i < 16; ++i) _cache[i] = _base[i];
        _last = 15;
        return;
    }

    if ((idx_t)n + 1 <= _cap) return;    /* already large enough */

    /* Grow the cache – exponential growth keeps amortised O(1) */
    idx_t newcap = _cap;
    while ((idx_t)n + 1 > newcap) newcap *= 2;
    seq_t *tmp = (seq_t*)realloc(_cache, (size_t)newcap * sizeof(seq_t));
    if (tmp == NULL) return;            /* realloc failed – abort silently */
    _cache = tmp;
    _cap   = newcap;
}

/* Compute a(n) for any n >= 0 (any call order) */
seq_t a(idx_t n)
{
    if (n < 16) return _base[n];

    _ensure_cache(n);

    /* Compute missing entries sequentially */
    for (idx_t k = _last + 1; k <= n; ++k) {
        seq_t t8  = _cache[k - 8];
        seq_t t16 = _cache[k - 16];
        _cache[k] = (seq_t)39202 * t8 - t16;
    }
    _last = n;
    return _cache[n];
}
