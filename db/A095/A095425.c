/*
 * A095425  a(n) = 10 written in base n.
 * https://oeis.org/A095425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 written in base n
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base‑1 special case: 10 is written as ten 1’s */
    if (n == 1u)
        return (seq_t)1111111111U;

    /* Compute the digits of decimal 10 in base n */
    uint64_t value  = 10;
    uint64_t base   = (uint64_t)n;
    uint64_t digits[16];
    int len = 0;

    while (value > 0)
    {
        digits[len++] = value % base;
        value /= base;
    }

    /* Concatenate the digits as decimal digits */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        uint64_t d = digits[i];
        if (d < 10)
            result = result * 10 + (seq_t)d;
        else            /* d == 10 */
            result = result * 100 + (seq_t)d;
    }

    return result;
}
