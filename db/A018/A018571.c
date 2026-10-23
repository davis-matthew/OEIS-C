/*
 * A018571  Divisors of 650.
 * https://oeis.org/A018571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth positive divisor of 650 in ascending order
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of divisors of 650 */
static seq_t _divisors[12];
static size_t _div_count = 0;

/* Initialize and sort the divisor cache */
static void _init_divisors(void)
{
    if (_div_count != 0)
        return;

    const unsigned int N = 650;
    size_t idx = 0;
    unsigned int i;

    for (i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            _divisors[idx++] = (seq_t)i;
            if (i != N / i)
                _divisors[idx++] = (seq_t)(N / i);
        }
    }

    /* Simple insertion sort: n = 12, trivial overhead */
    for (i = 0; i < idx - 1; ++i) {
        size_t j;
        for (j = i + 1; j < idx; ++j) {
            if (_divisors[j] < _divisors[i]) {
                seq_t tmp = _divisors[i];
                _divisors[i] = _divisors[j];
                _divisors[j] = tmp;
            }
        }
    }

    _div_count = idx;
}

/* Public function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; return 0 for 0 or out-of-bounds */
        return (seq_t)0;

    _init_divisors();

    size_t idx = (size_t)(n - 1);
    if (idx >= _div_count)
        return (seq_t)0;

    return _divisors[idx];
}
