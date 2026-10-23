/*
 * A018680  Divisors of 847.
 * https://oeis.org/A018680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 847 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t _divisors[6];
static int _divisors_inited = 0;

static void _init_divisors(void)
{
    /* 847 = 7 · 11². Generate all divisors from these prime powers. */
    seq_t p7[2]  = { (seq_t)1, (seq_t)7 };          /* 7⁰, 7¹  */
    seq_t p11[3] = { (seq_t)1, (seq_t)11, (seq_t)121 }; /* 11⁰, 11¹, 11² */

    size_t k = 0;
    for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 3; ++j)
            _divisors[k++] = p7[i] * p11[j];

    /* simple insertion sort – only 6 elements */
    for (size_t i = 1; i < 6; ++i) {
        seq_t key = _divisors[i];
        size_t j = i;
        while (j > 0 && _divisors[j - 1] > key) {
            _divisors[j] = _divisors[j - 1];
            --j;
        }
        _divisors[j] = key;
    }

    _divisors_inited = 1;
}

seq_t a(idx_t n)
{
    if (!_divisors_inited)
        _init_divisors();

    if (n == 0 || n > 6)
        return (seq_t)0;          /* out of range – not defined in OEIS */

    return _divisors[n - 1];
}
