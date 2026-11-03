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
 * Validated: standard, pass, 40 terms through n = 175 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to safely multiply a seq_t by a small unsigned constant.
   For builtin integer types (< 9 bytes) we use 128‑bit arithmetic to
   avoid overflow.  For arbitrary‐precision types we rely on the
   overloaded operator. */
static inline seq_t _mul_small(seq_t v, unsigned int m)
{
#if defined(__SIZEOF_INT128__) && !defined(__INT128__) && !defined(__int128__)
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((__uint128_t)v * m);
    }
#endif
    return v * m;
}

/* Compute one recurrence step using safe arithmetic. */
static inline seq_t _compute_step(seq_t prev1, seq_t prev2, seq_t prev3)
{
#if defined(__SIZEOF_INT128__) && !defined(__INT128__) && !defined(__int128__)
    if (sizeof(seq_t) <= 8) {
        __uint128_t res = (__uint128_t)prev1 * 4;
        res += (__uint128_t)prev3 * 2;
        res -= (__uint128_t)prev2 * 2;
        return (seq_t)res;
    }
#endif
    seq_t res = _mul_small(prev1, 4);
    res += _mul_small(prev3, 2);
    res -= _mul_small(prev2, 2);
    return res;
}

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
        _cache = mem_new(seq_t, _cache_size);
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
    seq_t * tmp = mem_resize(seq_t, _cache, new_size);
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
