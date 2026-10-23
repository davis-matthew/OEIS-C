/*
 * A042060  Numerators of continued fraction convergents to sqrt(554).
 * https://oeis.org/A042060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 348586*a(n-11) + a(n-22);
   initial values a(0) … a(21) are given below. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* 22 initial terms of the sequence */
static const seq_t _init[22] = {
    (seq_t)23,  (seq_t)24,  (seq_t)47,  (seq_t)306,  (seq_t)1271,
    (seq_t)1577, (seq_t)2848, (seq_t)12969, (seq_t)80662, (seq_t)93631,
    (seq_t)174293, (seq_t)8111109, (seq_t)8285402, (seq_t)16396511,
    (seq_t)106664468, (seq_t)443054383, (seq_t)549718851, (seq_t)992773234,
    (seq_t)4520811787, (seq_t)28117643956, (seq_t)32638455743, (seq_t)60756099699
};

/* Cache of already computed terms */
static seq_t * _cache    = NULL;   /* pointer to storage */
static idx_t   _cached_n = 0;      /* number of terms currently stored */
static idx_t   _cache_cap = 0;     /* capacity of the storage */

/* Ensure the cache can hold at least "size" terms */
static void _ensure_capacity(idx_t size)
{
    if (size <= _cache_cap) return;

    idx_t new_cap = _cache_cap ? _cache_cap * 2 : 22;
    while (new_cap < size) new_cap *= 2;

#ifdef __cplusplus
    /* Use C++ new[]/delete[] for class types */
    seq_t *new_cache = new seq_t[new_cap];
    if (_cache) {
        std::copy(_cache, _cache + _cached_n, new_cache);
        delete [] _cache;
    }
    _cache = new_cache;
#else
    /* In C, realloc works for POD types */
    void *tmp = realloc(_cache, sizeof(seq_t) * new_cap);
    /* undef: if realloc fails and returns NULL, the cache is lost.
       The program will terminate on subsequent use. */
    _cache = (seq_t*)tmp;
#endif
    _cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (_cache == NULL) {
        _ensure_capacity(22);
        for (idx_t i = 0; i < 22; ++i) _cache[i] = _init[i];
        _cached_n = 22;
    }

    /* Grow cache if the requested n is not yet computed */
    if (n + 1 > _cached_n) {
        idx_t need = n + 1;
        _ensure_capacity(need);
        for (idx_t k = _cached_n; k <= n; ++k) {
            seq_t term = (seq_t)348586 * _cache[k - 11] + _cache[k - 22];
            _cache[k] = term;
        }
        _cached_n = need;
    }

    return _cache[n];
}
