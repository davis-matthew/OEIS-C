/*
 * A021137  Decimal expansion of 1/133.
 * https://oeis.org/A021137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/133) mod 10 (decimal digit of 1/133)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[64];          /* period will be at most 64 digits */
    static size_t period_len = 0;     /* length of the repeating block */

    if (period_len == 0)
    {
        /* compute the period of 1/133 by long division */
        int r = 1;          /* initial remainder */
        size_t i = 0;
        while (1)
        {
            r *= 10;
            int digit = r / 133;
            r %= 133;
            period[i++] = (seq_t)digit;
            if (r == 1)    /* cycle returns to the starting remainder */
            {
                period_len = i;
                break;
            }
        }
    }

    size_t idx = (size_t)(n % ((idx_t)period_len));
    return period[idx];
}
