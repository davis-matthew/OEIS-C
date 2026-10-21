/*
 * A230602  a(n) = Lucas(2^n - 2).
 * https://oeis.org/A230602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2^n - 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached state, valid for any order of calls. */
static idx_t _a_current = 0;          /* largest n for which elements are cached */
static seq_t *_a_cache = NULL;        /* array 1‑based, _a_cache[1] = a(1) */
static size_t _a_cache_cap = 0;       /* current allocated capacity */

static seq_t _Lk = 0;  /* L_k where k = 2^{n-1} – 2   */
static seq_t _Lk1 = 0; /* L_{k+1} */

/* Ensure cache has at least n elements; generate missing terms. */
static void _a_prepare(idx_t n)
{
    if (_a_current == 0) {
        /* Initialise for a(1)=L_0=2, L_1=1.  Allocate small capacity. */
        _a_current = 1;
        _Lk = 2;          /* L_0 */
        _Lk1 = 1;         /* L_1 */
        _a_cache_cap = 8;
        _a_cache = (seq_t *)malloc(_a_cache_cap * sizeof(seq_t));
        _a_cache[1] = 2;  /* a(1) */
    }

    /* Grow cache array if needed. */
    if (n >= _a_cache_cap) {
        while (_a_cache_cap <= n)
            _a_cache_cap <<= 1;
        _a_cache = (seq_t *)realloc(_a_cache, _a_cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n. */
    while (_a_current < n) {
        /* a(n+1) = L_{k+1}^2 + 2  (k is always even). */
        seq_t next = _Lk1 * _Lk1 + 2;

        /* Update Lucas pair for the next step.
           L_{k_next}   = next                (equals L_{k+1}^2 + 2)
           L_{k_next+1} = L_{k+1}*(L_{k+1}+L_k) + 1   (== L_{k+2}*L_{k+1} + 1) */
        seq_t tmp = _Lk1 + _Lk;
        seq_t new_Lk1 = _Lk1 * tmp + 1;
        seq_t new_Lk  = next;                /* also L_{k_next} */

        /* Store the new term and advance state. */
        _a_cache[++_a_current] = next;
        _Lk  = new_Lk;
        _Lk1 = new_Lk1;
    }
}

seq_t a(idx_t n)
{
    _a_prepare(n);
    return _a_cache[n];
}
