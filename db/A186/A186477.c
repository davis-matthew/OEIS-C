/*
 * A186477  Number of (n+1)X5 0..3 arrays with every 2X2 subblock commuting
 *          with each of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A186477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1)+2*a(n‑2)−6*a(n‑3)+6*a(n‑4)−3*a(n‑5) for n>15
 *            Initial values: a(1)=1251, a(2)=2049, a(3)=4810, a(4)=16762,
 *            a(5)=25633, a(6)=67714, a(7)=101305, a(8)=286290, a(9)=456113,
 *            a(10)=1219034, a(11)=2037173, a(12)=5189546, a(13)=9017049,
 *            a(14)=22136029, a(15)=39734818
 * Range:     u8 none, u16 n <= 5, u32 n <= 21, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* constant initial part of the sequence */
static const seq_t _init_vals[15] = {
    (seq_t)1251, (seq_t)2049, (seq_t)4810, (seq_t)16762, (seq_t)25633,
    (seq_t)67714, (seq_t)101305, (seq_t)286290, (seq_t)456113,
    (seq_t)1219034, (seq_t)2037173, (seq_t)5189546, (seq_t)9017049,
    (seq_t)22136029, (seq_t)39734818
};

/* cache for computed values */
static seq_t * _cache = NULL;      /* dynamic array of a(n) values */
static size_t   _cache_cap = 0;    /* allocated capacity (terms) */
static size_t   _cache_len = 0;    /* number of terms already computed */

/* ensure the cache array has room for at least n terms  (n>=1) */
static void _ensure_cache(idx_t n)
{
    if (_cache_cap < (size_t)n) {
        size_t new_cap = _cache_cap ? _cache_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }          /* unlikely; handle bad alloc */
        _cache = tmp;
        _cache_cap = new_cap;
    }
}

/* compute a(n) ;  n>=1 (a(0) is undefined and returns 0) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset defined from n=1 */

    _ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(_cache_len + 1); i <= n; ++i) {
        seq_t val;
        if (i <= 15) {                     /* base values from table */
            val = _init_vals[i - 1];
        } else {                           /* recurrence for i>15 */
            /* cache[j] holds a(j+1), so a(i‑k) -> cache[i‑k‑1] */
            seq_t a1 = _cache[i - 2];       /* a(i-1) */
            seq_t a2 = _cache[i - 3];       /* a(i-2) */
            seq_t a3 = _cache[i - 4];       /* a(i-3) */
            seq_t a4 = _cache[i - 5];       /* a(i-4) */
            seq_t a5 = _cache[i - 6];       /* a(i-5) */
            val = a1 * 2 + a2 * 2 - a3 * 6 + a4 * 6 - a5 * 3;
        }
        _cache[i - 1] = val;                /* store a(i) */
    }

    _cache_len = (size_t)n;
    return _cache[n - 1];
}
