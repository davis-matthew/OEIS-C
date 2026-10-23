/*
 * A029748  Numbers of the form 2^k times 1, 3 or 7.
 * https://oeis.org/A029748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) for n > 5 Initial values: a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=4, a(4)=6, a(5)=7 Computes the sequence of numbers
 *            of the form 2^k * {1,3,7}
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 92, u64 n <= 188,
 *            u128 n <= 380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Precomputed base values */
static const seq_t _base[6] = {1, 2, 3, 4, 6, 7};

/* Cached sequence */
static seq_t * _cache = NULL;      /* pointer to allocated array   */
static size_t  _cap   = 0;          /* allocated capacity (size_t)  */
static idx_t   _max   = 0;          /* highest index cached         */

/* Ensure the cache contains terms up to index n (inclusive) */
static void __ensure_cache(idx_t n)
{
    /* Expand capacity if necessary */
    if (n + 1 > (idx_t)_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap == 0) new_cap = 1;          /* guard against n = ULLONG_MAX? */
        if (_cap == 0) {
            _cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            _cache = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        }
        _cap = new_cap;
    }

    /* Fill in missing values */
    /* If cache just created, fill initial 6 elements */
    if (_max == 0 && _cap > 0) {
        size_t i;
        for (i = 0; i < 6 && i < _cap; ++i) {
            _cache[i] = _base[i];
        }
        _max = i - 1;
    }

    /* Extend from _max+1 up to n */
    idx_t start = _max + 1;
    if (start < 6) start = 6;   /* base case indices already set */
    for (idx_t i = start; i <= n; ++i) {
        /* i-3 is guaranteed to be computed because i >= 6 */
        _cache[i] = 2 * _cache[i - 3];
    }
    if (n > _max) {
        _max = n;
    }
}

seq_t a(idx_t n)
{
    /* Compute term a(n) using memoized recurrence */
    if (_cache == NULL) {
        /* Prime the cache: allocate minimal capacity */
        size_t init_cap = 6;
        _cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        _cap = init_cap;
        size_t i;
        for (i = 0; i < 6; ++i) {
            _cache[i] = _base[i];
        }
        _max = 5;
    }

    if (n > (idx_t)_max) {
        __ensure_cache(n);
    }
    return _cache[n];
}
