/*
 * A018405  Divisors of 348.
 * https://oeis.org/A018405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 348 in increasing order
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t _divisors[12];
static size_t _divcount = 0;
static bool _divinit = false;

static void _init_divisors(void)
{
    /* generate all divisors of 348 in ascending order */
    const idx_t N = (idx_t)348;
    idx_t i;
    for (i = 1; i <= N; ++i) {
        if (N % i == 0) {
            _divisors[_divcount++] = (seq_t)i;
        }
    }
    /* _divcount will be 12 for 348 */
    _divinit = true;
}

seq_t a(idx_t n)
{
    if (!_divinit) {
        _init_divisors();
    }
    if (n >= 1 && n <= _divcount) {
        return _divisors[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
