/*
 * A095645  a(n) = 127 written in base 11 - n.
 * https://oeis.org/A095645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value obtained by writing 127 as a base-(11-n)
 *            number and interpreting the digits as a decimal integer
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Only defined for 0 <= n <= 10.  For n > 10, return 0. */
    if (n > 10)
        return (seq_t)0;

    int base = 11 - (int)n;

    /* Base 1 (unary) is handled separately – 127 ones. */
    if (base == 1)
    {
        seq_t res = 0;
        for (int i = 0; i < 127; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Convert 127 to the given base. */
    int m = 127;
    int digits[20];
    int len = 0;
    while (m > 0)
    {
        digits[len++] = m % base;
        m /= base;
    }

    /* Build the decimal number formed by those digits. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
