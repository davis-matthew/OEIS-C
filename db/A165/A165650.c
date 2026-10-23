/*
 * A165650  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence derived from the rational generating function
 *            * a(n) = 33*a(n-1) + 33*a(n-2) + … + 33*a(n-8) – 561*a(n-9) (n ≥
 *            10) * Initial values: a(0) … a(9) given below
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _base[10] = {
    (seq_t)1ULL,
    (seq_t)35ULL,
    (seq_t)1190ULL,
    (seq_t)40460ULL,
    (seq_t)1375640ULL,
    (seq_t)46771760ULL,
    (seq_t)1590239840ULL,
    (seq_t)54068154560ULL,
    (seq_t)1838317255040ULL,
    (seq_t)62502786670765ULL
};

static seq_t * _cache = NULL;      /* storage for computed terms */
static idx_t  _cache_max = 0;      /* highest index stored   */
static size_t _cache_cap  = 0;      /* allocated capacity     */

/* Expand the cache so that it can hold terms 0 … n */
static void _ensure_cache(idx_t n)
{
    if (_cache_cap == 0) {   /* first allocation */
        _cache_cap = 16;                   /* arbitrary initial size */
        _cache     = (seq_t*)malloc(_cache_cap * sizeof(seq_t));
        if (!_cache) abort();
        /* initialise the first ten terms */
        for (idx_t i = 0; i < 10; ++i) _cache[i] = _base[i];
        _cache_max = 9;
    }

    if (n <= _cache_max) return;   /* nothing to do */

    /* extend the allocation if necessary */
    if (_cache_cap <= n) {
        size_t new_cap = _cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        _cache     = tmp;
        _cache_cap = new_cap;
    }

    /* compute new terms using the recurrence */
    for (idx_t i = _cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 1; j <= 8; ++j)      /* +33*a(n-j) */
            sum += _cache[i - j] * (seq_t)33;
        sum -= _cache[i - 9] * (seq_t)561; /* -561*a(n-9)  */
        _cache[i] = sum;
    }
    _cache_max = n;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n < 10) return _base[n];
    _ensure_cache(n);
    return _cache[n];
}
