/*
 * A224661  Number of n X n 0..4 matrices with each 2X2 subblock idempotent.
 * https://oeis.org/A224661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224661
    Formula (n > 10):  a(n) = 6*a(n-1) - 12*a(n-2) + 5*a(n-3) + 12*a(n-4)
                       - 12*a(n-5) - 3*a(n-6) + 6*a(n-7) - a(n-9)
    Initial values:  a(1)=5, a(2)=20, a(3)=98, a(4)=180, a(5)=412,
                     a(6)=806, a(7)=1606, a(8)=3070, a(9)=5808,
                     a(10)=10798, a(11)=19866, a(12)=36184
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

static const seq_t _first[12] = {
    (seq_t)5,   (seq_t)20,  (seq_t)98,  (seq_t)180,
    (seq_t)412, (seq_t)806, (seq_t)1606,(seq_t)3070,
    (seq_t)5808,(seq_t)10798,(seq_t)19866,(seq_t)36184
};

static seq_t * _cache = NULL;    /* allocated array */
static size_t  _cache_cap = 0;   /* capacity of the array */
static size_t  _cache_last = 0;  /* highest index (1‑based) computed */

/* ensure the cache has space for at least n elements (1‑based) */
static void _ensure_cache(size_t n)
{
    if (n <= _cache_cap) return;
    size_t new_cap = _cache_cap? _cache_cap * 2 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    _cache = tmp;
    _cache_cap = new_cap;
}

/* compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* very small indices are handled directly in the table */
    if (n <= 12) return _first[(size_t)n - 1];

    /* initialise the cache with the first 12 values */
    if (_cache == NULL) {
        _ensure_cache(12);
        for (size_t i = 0; i < 12; ++i) _cache[i] = _first[i];
        _cache_last = 12;
    }

    /* compute any missing terms up to n */
    if ((size_t)n > _cache_last) {
        _ensure_cache((size_t)n);
        for (size_t i = _cache_last + 1; i <= (size_t)n; ++i) {
            /* recurrence indices are 1‑based, cache indices are 0‑based */
            seq_t val =  6 * _cache[i-2]        /* a(i-1) */
                       -12 * _cache[i-3]        /* a(i-2) */
                       + 5 * _cache[i-4]        /* a(i-3) */
                       +12 * _cache[i-5]        /* a(i-4) */
                       -12 * _cache[i-6]        /* a(i-5) */
                       - 3 * _cache[i-7]        /* a(i-6) */
                       + 6 * _cache[i-8]        /* a(i-7) */
                       -   _cache[i-10];        /* a(i-9) */
            _cache[i-1] = val;
        }
        _cache_last = (size_t)n;
    }

    return _cache[(size_t)n - 1];
}
