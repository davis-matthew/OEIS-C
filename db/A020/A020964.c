/*
 * A020964  a(n) = Sum_{k>=1} floor( 3*(1+sqrt(2))^(n-k) ).
 * https://oeis.org/A020964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=11, a(3)=28, a(4)=70, a(5)=171, a(6)=417;
 *            Recurrence: for n>6 a(n) = 3*a(n-1) - 4*a(n-3) + a(n-4) + a(n-5)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _BASE[7] = {1, 4, 11, 28, 70, 171, 417};

static seq_t * _cache = NULL;
static idx_t   _cache_size = 0;
static idx_t   _last_computed = 0;   /* highest index that has a valid value in _cache */

seq_t a(idx_t n)
{
    /* Direct value for the first seven indices */
    if (n < 7) {
        return _BASE[n];
    }

    /* Allocate or grow the cache if needed */
    if (_cache == NULL) {
        _cache_size = n + 1;
        _cache = (seq_t *)malloc((size_t)_cache_size * sizeof(seq_t));
        if (_cache == NULL) return 0;
        for (idx_t i = 0; i < 7; ++i) {
            _cache[i] = _BASE[i];
        }
        _last_computed = 6;
    } else if (n + 1 > _cache_size) {
        _cache_size = n + 1;
        _cache = (seq_t *)realloc(_cache, (size_t)_cache_size * sizeof(seq_t));
        if (_cache == NULL) return 0;
        if (_last_computed < 6) {
            for (idx_t i = 0; i < 7; ++i) {
                _cache[i] = _BASE[i];
            }
            _last_computed = 6;
        }
    }

    /* Compute missing values up to n */
    if (n > _last_computed) {
        for (idx_t i = _last_computed + 1; i <= n; ++i) {
            seq_t val = (seq_t)3 * _cache[i - 1];
            val -= (seq_t)4 * _cache[i - 3];
            val += _cache[i - 4];
            val += _cache[i - 5];
            _cache[i] = val;
        }
        _last_computed = n;
    }

    return _cache[n];
}
