/*
 * A079269  Define b by b(1) = 1 and for n>1, b(n) = b(n-1)+1/(1+1/b(n-1));
 *          sequence gives numerator of b(n).
 * https://oeis.org/A079269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079269 – numerator of the rational sequence b(n)
 * b(1) = 1
 * b(n) = b(n‑1) + 1/(1 + 1/b(n‑1))
 *
 * Recurrence for the numerators (p) and denominators (q) of b(n):
 *   p(1) = 1 , q(1) = 1
 *   p(n) = p(n‑1) * (p(n‑1) + 2·q(n‑1))
 *   q(n) = q(n‑1) * (p(n‑1) + q(n‑1))
 *
 * a(n) = p(n)  (the numerator)
 *
 * Initial values used: a(1) = 1
 * The function memoises all computed terms up to the largest n
 * requested, making repeated calls with arbitrary ordering efficient.
 */
#include <stdlib.h>

static size_t _cache_size = 0;    /* number of terms already in cache */
static size_t _cache_cap  = 0;    /* allocated capacity of the cache          */
static seq_t * _p_cache = NULL;   /* numerators p(n)                          */
static seq_t * _q_cache = NULL;   /* denominators q(n)                        */

static void
_ensure(idx_t n)
{
    if ((idx_t)_cache_size >= n) return;          /* already computed */

    /* grow the cache if needed */
    if (_cache_cap == 0) {
        _cache_cap = 1;
        _p_cache = (seq_t*)malloc(sizeof(seq_t) * _cache_cap);
        _q_cache = (seq_t*)malloc(sizeof(seq_t) * _cache_cap);
    }
    while (_cache_cap < (size_t)n) {
        size_t new_cap = _cache_cap * 2;
        _p_cache = (seq_t*)realloc(_p_cache, sizeof(seq_t) * new_cap);
        _q_cache = (seq_t*)realloc(_q_cache, sizeof(seq_t) * new_cap);
        _cache_cap = new_cap;
    }

    /* compute terms until we reach n */
    while (_cache_size < n) {
        if (_cache_size == 0) {
            /* b(1) = 1/1 */
            _p_cache[0] = (seq_t)1;
            _q_cache[0] = (seq_t)1;
        } else {
            seq_t p_prev = _p_cache[_cache_size - 1];
            seq_t q_prev = _q_cache[_cache_size - 1];

            /* temp = p_prev + 2*q_prev */
            seq_t twoq   = q_prev + q_prev;
            seq_t temp   = p_prev + twoq;

            /* sum_pq = p_prev + q_prev */
            seq_t sum_pq = p_prev + q_prev;

            /* new numerator and denominator */
            seq_t p_new = p_prev * temp;
            seq_t q_new = q_prev * sum_pq;

            _p_cache[_cache_size] = p_new;
            _q_cache[_cache_size] = q_new;
        }
        _cache_size++;
    }
}

seq_t
a(idx_t n)
{
    /* a(1) is the first element; behaviour for n < 1 is undefined */
    _ensure(n);
    return _p_cache[n - 1];
}
