/*
 * A219737  Unmatched value maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          vertical or antidiagonal neighbor in a random 0..1 n X 4 array.
 * https://oeis.org/A219737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2) + 15*a(n-3) + 4*a(n-4) * - 6*a(n-5) -
 *            a(n-6) + 3*a(n-7) - a(n-8), n>9 * Initial values: * a(1)=7,
 *            a(2)=28, a(3)=126, a(4)=524, a(5)=2229, a(6)=9425, * a(7)=39905,
 *            a(8)=168925, a(9)=715072
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Base values for n = 1 … 9.  Index 0 is unused. */
static const seq_t _base[10] = {
    0ull,   /* placeholder for index 0 */
    7ull, 28ull, 126ull, 524ull, 2229ull,
    9425ull, 39905ull, 168925ull, 715072ull
};

/* Static cache for computed terms.  cache[0] is unused. */
static seq_t * _cache      = NULL;
static size_t _cache_size  = 0;      /* allocated size (including index 0) */
static idx_t   _max_computed = 0;    /* maximum n whose term is computed */

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined but safe */

    /* Grow the cache if necessary */
    size_t needed = (size_t)n + 1;     /* +1 because index 0 unused */
    if (needed > _cache_size) {
        size_t new_size = needed;
        seq_t *tmp = (seq_t *)realloc(_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        _cache      = tmp;
        _cache_size = new_size;
    }

    /* Fill base cases up to min(n,9) if not already present */
    if (_max_computed < 9) {
        idx_t up_to = (_max_computed < 9) ? 9 : _max_computed;
        if (up_to > n) up_to = n;
        for (idx_t i = _max_computed + 1; i <= up_to; ++i)
            _cache[i] = _base[i];
        _max_computed = up_to;
    }

    if (n <= 9) /* already computed */
        return _cache[n];

    /* Compute terms that are not yet cached using recurrence */
    for (idx_t i = _max_computed + 1; i <= n; ++i) {
        _cache[i] =
            _cache[i-1]
          + (seq_t)10 * _cache[i-2]
          + (seq_t)15 * _cache[i-3]
          + (seq_t) 4 * _cache[i-4]
          - (seq_t) 6 * _cache[i-5]
          - _cache[i-6]
          + (seq_t) 3 * _cache[i-7]
          - _cache[i-8];
    }

    _max_computed = n;
    return _cache[n];
}
