/*
 * A177500  Divisors of 8064.
 * https://oeis.org/A177500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = n-th divisor of 8064 in increasing order
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Static array to hold all divisors of 8064 in ascending order */
static seq_t _divisors[48];
static size_t _div_count = 0;

/* Initialize the divisor array once */
static void _init_divisors(void)
{
    if (_div_count != 0)
        return;                       /* already initialized */

    const uint64_t N = 8064;          /* 2^7 * 3^2 * 7 */

    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            _divisors[_div_count++] = (seq_t)i;          /* small divisor */
            uint64_t j = N / i;
            if (j != i)                                  /* large divisor */
                _divisors[_div_count++] = (seq_t)j;
        }
    }

    /* Simple insertion sort on the small array of at most 48 elements */
    for (size_t i = 1; i < _div_count; ++i) {
        seq_t key = _divisors[i];
        size_t j = i;
        while (j > 0 && _divisors[j - 1] > key) {
            _divisors[j] = _divisors[j - 1];
            --j;
        }
        _divisors[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for n < 1 */

    _init_divisors();

    /* n is 1-based; array index is n-1 */
    if ((idx_t)(n - 1) < _div_count)
        return _divisors[n - 1];

    /* For n beyond the number of divisors, return 0 */
    return 0;
}
