/*
 * A037635  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3.
 * https://oeis.org/A037635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit[0]*7^(n-1) + digit[1]*7^(n-2) + ... + digit[n-1],
 *            where digit[i] follows the periodic sequence 2,1,3 repeating
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const int digits[3] = { 2, 1, 3 };
    seq_t value = 0;

    for (idx_t i = 0; i < n; ++i)
    {
        value = value * ((seq_t)7) + (seq_t)digits[i % 3];
    }

    return value;
}
