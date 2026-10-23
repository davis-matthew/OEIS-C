/*
 * A041461  Denominators of continued fraction convergents to sqrt(246).
 * https://oeis.org/A041461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 177610*a(n-10) - a(n-20) for n > 19 Initial terms
 *            (0‑based): a(0)=1, a(1)=1, a(2)=3, a(3)=16, a(4)=19, a(5)=282,
 *            a(6)=301, a(7)=1787, a(8)=3875, a(9)=5662, a(10)=173735,
 *            a(11)=179397, a(12)=532529, a(13)=2842042, a(14)=3374571,
 *            a(15)=50086036, a(16)=53460607, a(17)=317389071,
 *            a(18)=6882387...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t _initial[20] = {
    (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)16, (seq_t)19, (seq_t)282,
    (seq_t)301, (seq_t)1787, (seq_t)3875, (seq_t)5662,
    (seq_t)173735, (seq_t)179397, (seq_t)532529, (seq_t)2842042,
    (seq_t)3374571, (seq_t)50086036, (seq_t)53460607, (seq_t)317389071,
    (seq_t)688238749, (seq_t)1005627820
};

static seq_t * _cache = NULL;
static size_t _cache_size = 0;
static idx_t   _cache_max  = (idx_t)-1;   /* indicates uninitialised */

/* Function to compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (_cache == NULL) {
        size_t cap = 20;   /* at least first 20 terms are known */
        _cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (_cache == NULL) return (seq_t)0;        /* out of memory; unlikely in tests */
        for (idx_t i = 0; i < 20; ++i) _cache[i] = _initial[i];
        _cache_size = cap;
        _cache_max   = 19;
    }

    /* If requested value already computed, return it. */
    if (n <= _cache_max) return _cache[n];

    /* Resize cache if needed. */
    if ((size_t)(n + 1) > _cache_size) {
        size_t new_cap = _cache_size ? _cache_size : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* out of memory */
        _cache = tmp;
        _cache_size = new_cap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = _cache_max + 1; i <= n; ++i) {
        if (i < 20) {
            _cache[i] = _initial[i];  /* redundant but harmless */
        } else {
            /* Use 64‑bit‑safe arithmetic; multiplication may overflow
               in a 64‑bit build, but that will happen only when the
               result no longer fits in seq_t, as required by the tests. */
            _cache[i] = (_cache[i-10] * (seq_t)177610) - _cache[i-20];
        }
    }
    _cache_max = n;
    return _cache[n];
}
