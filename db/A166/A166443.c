/*
 * A166443  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A166443: Number of reduced words of length n
   in Coxeter group on 49 generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
   Integer formula: a(0) = 1, a(1) = 49, ..., a(10)=66277667569139712.
   Recurrence (derived from the generating function):
       a(11) = 47*∑_{j=1}^{10} a(j)  - 1128*a(0)  + 1
       a(n)  = 47*∑_{j=1}^{10} a(n-j) - 1128*a(n-11)   for n ≥ 12
   The '+1' term only appears in the n = 11 case.  All other terms satisfy
   the homogeneous linear recurrence.  The implementation below caches
   previously computed terms so that repeated calls in arbitrary order are
   efficient.  It works for any unsigned integer type seq_t (including
   arbitrary precision) and any unsigned 64‑bit index type idx_t. */

#include <stdlib.h>

/* initial 11 values (a(0) … a(10)) */
static const seq_t _init[11] = {
    (seq_t)1ULL,
    (seq_t)49ULL,
    (seq_t)2352ULL,
    (seq_t)112896ULL,
    (seq_t)5419008ULL,
    (seq_t)260112384ULL,
    (seq_t)12485394432ULL,
    (seq_t)599298932736ULL,
    (seq_t)28766348771328ULL,
    (seq_t)1380784741023744ULL,
    (seq_t)66277667569139712ULL
};

/* cached sequence values */
static seq_t * _cache = NULL;
static idx_t   _cap   = 0;
static idx_t   _sz    = 0;

/* external linkage as required */
seq_t a(idx_t n)
{
    /* direct return of the first 11 terms */
    if (n < 11)
        return _init[n];

    /* initialise cache on first use */
    if (_cache == NULL) {
        _cap = 11;
        _cache = (seq_t*)malloc(_cap * sizeof(seq_t));
        if (_cache == NULL) return (seq_t)0; /* allocation failed */
        for (idx_t i = 0; i < 11; ++i) _cache[i] = _init[i];
        _sz = 11;
    }

    /* if requested value already cached, return it */
    if (n < _sz) return _cache[n];

    /* grow cache if needed */
    if (n >= _cap) {
        idx_t new_cap = _cap ? _cap : 11;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        _cache = tmp;
        _cap = new_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = _sz; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 10; ++j)
            sum += _cache[i - j];

        if (i == 11) {
            /* special inhomogeneous term for n = 11 */
            _cache[i] = sum * 47 - _cache[0] * 1128 + (seq_t)1;
        } else {
            _cache[i] = sum * 47 - _cache[i - 11] * 1128;
        }
    }

    _sz = n + 1;
    return _cache[n];
}
