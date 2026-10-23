/*
 * A018272  Divisors of 75.
 * https://oeis.org/A018272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 75 in increasing order (a(1)=1,
 *            a(2)=3, … a(6)=75)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool _a018272_initialized = false;
static seq_t _a018272_divisors[6];

static void _a018272_init(void)
{
    /* 75 = 3^1 * 5^2.  Generate divisors from all combinations of the
       prime powers, then sort the resulting array. */
    const seq_t p3[2] = {1, 3};
    const seq_t p5[3] = {1, 5, 25};
    seq_t temp[6];
    int idx = 0;
    for (int i = 0; i < 2; ++i)          /* exponent of 3: 0 or 1 */
        for (int j = 0; j < 3; ++j)      /* exponent of 5: 0,1,2 */
            temp[idx++] = p3[i] * p5[j];

    /* Simple insertion sort for 6 elements */
    for (int i = 1; i < 6; ++i) {
        seq_t key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            --j;
        }
        temp[j + 1] = key;
    }

    for (int i = 0; i < 6; ++i)
        _a018272_divisors[i] = temp[i];

    _a018272_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        /* OEIS offset is 1; undefined for n=0 – return 0 */
        return (seq_t)0u;

    if (!_a018272_initialized)
        _a018272_init();

    if (n <= 6)
        return _a018272_divisors[n - 1];
    else
        /* Sequence has only 6 terms; values beyond are undefined – return 0 */
        return (seq_t)0u;
}
