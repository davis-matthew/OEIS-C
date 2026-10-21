/*
 * A239266  Number of domicule tilings of a 4 X n grid.
 * https://oeis.org/A239266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239266: number of domicule tilings of a 4×n grid
 *
 * The rational generating function is
 *   G(x) = -(x-1)(x^3-x^2+5x-1) / (5x^6-11x^5+30x^4-30x^3-2x^2+7x-1).
 *
 * From this a linear recurrence with constant coefficients follows:
 *
 *   a(0) = 1
 *   a(1) = 1
 *   a(2) = 11
 *   a(3) = 43
 *
 *   for all n ≥ 0
 *       a(n) = 7·a(n-1) - 2·a(n-2) - 30·a(n-3)
 *              + 30·a(n-4) - 11·a(n-5) + 5·a(n-6) - p(n)
 *
 *   where the auxiliary sequence p(n) is
 *        p(0) = –1,  p(1) = 6,  p(2) = –6,  p(3) = 2,  p(4) = –1,
 *        p(n) = 0 for all n ≥ 5.
 *
 * The recurrence requires terms up to a(n‑6).  When these terms do not
 * exist (i.e. for n < 6) they are treated as zero.  The p(n) term
 * adjusts the first few values and disappears for n ≥ 5.
 *
 * The function below memoises results in a static dynamic array so
 * that arbitrary order of calls and repeated evaluations are
 * efficient.  All arithmetic uses only the operators permitted by
 * the build and therefore works for any unsigned seq_t (8, 16,
 * 32, 64, 128‑bit or arbitrary‑precision).
 */

#include <stdlib.h>

static seq_t *_cache = NULL;
static size_t _cap   = 0;
static size_t _len   = 0;           /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* helper: small const array for the p(n) adjustment   */
    static const int p_arr[5] = { -1,  6, -6,  2, -1 };

    /* initialise the cache once, with enough room for a few terms
       and store the first four values – the recurrence will
       compute a(4) and higher.                                   */
    if (_cache == NULL) {
        _cap = 32;
        _cache = (seq_t *)malloc(_cap * sizeof(seq_t));
        if (_cache == NULL) return (seq_t)0; /* allocation failure (unlikely) */

        /* the first four terms are known */
        _cache[0] = (seq_t)1;   /* a(0) */
        _cache[1] = (seq_t)1;   /* a(1) */
        _cache[2] = (seq_t)11;  /* a(2) */
        _cache[3] = (seq_t)43;  /* a(3) */

        _len = 4;
    }

    /* expand the array if necessary */
    if ((size_t)n+1 > _cap) {
        size_t new_cap = _cap ? _cap : 8;
        while (new_cap < (size_t)n+1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure (unlikely) */
        _cache = tmp;
        _cap = new_cap;
    }

    /* if we already have the requested value, return it */
    if ((size_t)n < _len)
        return _cache[n];

    /* compute missing terms up to n, in increasing order */
    for (idx_t i = _len; i <= n; ++i) {
        seq_t val = (seq_t)0;

        /* contributions from previous terms, missing ones are treated as 0   */
        if (i >= 1) val += _cache[i - 1] * (seq_t)7;
        if (i >= 2) val -= _cache[i - 2] * (seq_t)2;
        if (i >= 3) val -= _cache[i - 3] * (seq_t)30;
        if (i >= 4) val += _cache[i - 4] * (seq_t)30;
        if (i >= 5) val -= _cache[i - 5] * (seq_t)11;
        if (i >= 6) val += _cache[i - 6] * (seq_t)5;

        /* adjust by p(i) (non‑zero only for i = 0..4) */
        if (i <= 4) {
            int p = p_arr[(int)i];
            if (p > 0)      /* subtract positive p(i)   */
                val -= (seq_t)p;
            else if (p < 0) /* add |p(i)| for negative   */
                val += (seq_t)(-p);
        }

        _cache[i] = val;
    }

    _len = (size_t)n + 1;
    return _cache[n];
}
