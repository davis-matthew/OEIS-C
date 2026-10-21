/*
 * A224128  Number of n X 3 0..1 arrays with rows nondecreasing and
 *          antidiagonals unimodal.
 * https://oeis.org/A224128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + 2*a(n-3). Initial values: a(1)=4,
 *            a(2)=16, a(3)=58
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   _compute_step, _mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_compute_step.h"
#include "../utils/_mul_small.h"

/* Cached array of previously computed terms. */
static seq_t * _cache = NULL;
static size_t _cache_size = 0;   /* number of terms stored */

/* Return the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* Base cases.  idx_t is unsigned, so n==0 cannot occur. */
        static const seq_t base[3] = {4, 16, 58};
        return base[n-1];
    }

    /* Ensure cache has enough capacity. */
    if (_cache_size < 3) {
        _cache_size = 3;
        _cache = (seq_t*)malloc(_cache_size * sizeof(seq_t));
        if (!_cache) { /* allocation failure – abort */
            abort();
        }
        _cache[0] = 4;
        _cache[1] = 16;
        _cache[2] = 58;
    }

    if ((size_t)n <= _cache_size) {
        return _cache[n-1];
    }

    /* Extend cache to size n. */
    size_t new_size = (size_t)n;
    seq_t * tmp = (seq_t*)realloc(_cache, new_size * sizeof(seq_t));
    if (!tmp) {
        abort();
    }
    _cache = tmp;

    /* Compute missing terms. */
    for (size_t i = _cache_size; i < new_size; ++i) {
        /* i is zero‑based; corresponds to term number i+1. */
        seq_t v1 = _cache[i-1];        /* a(i)   */
        seq_t v2 = _cache[i-2];        /* a(i-1) */
        seq_t v3 = _cache[i-3];        /* a(i-2) */
        _cache[i] = _compute_step(v1, v2, v3);
    }

    _cache_size = new_size;
    return _cache[n-1];
}
