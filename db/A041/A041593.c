/*
 * A041593  Denominators of continued fraction convergents to sqrt(314).
 * https://oeis.org/A041593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 886*a(n-7) + a(n-14) for n > 13 * Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=4, a(4)=7, a(5)=18, a(6)=25, * a(7)=868,
 *            a(8)=893, a(9)=2654, a(10)=3547, a(11)=6201, a(12)=15949,
 *            a(13)=22150
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 48, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

static const seq_t _init_vals[14] = {
    1, 1, 3, 4, 7, 18, 25, 868, 893, 2654, 3547, 6201, 15949, 22150
};

#define CACHE_MAX 20000               /* enough for n up to about 10000 */

static seq_t _cache[CACHE_MAX];
static size_t _cache_size = 0;         /* number of terms currently cached */

seq_t a(idx_t n)
{
    /* initial cache fill */
    if (_cache_size == 0) {
        memcpy(_cache, _init_vals, sizeof(_init_vals));
        _cache_size = 14;
    }

    /* return if already computed */
    if ((size_t)n < _cache_size)
        return _cache[(size_t)n];

    /* compute missing terms up to n */
    for (size_t i = _cache_size; i <= (size_t)n; ++i) {
        _cache[i] = (seq_t)886 * _cache[i - 7] + _cache[i - 14];
    }
    _cache_size = (size_t)n + 1;

    return _cache[(size_t)n];
}
