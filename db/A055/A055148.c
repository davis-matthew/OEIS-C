/*
 * A055148  The first n digits of the juxtaposition of the base-7 numbers
 *          converted to decimal.
 * https://oeis.org/A055148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal integer represented by the first n digits of
 *            * the infinite string obtained by concatenating the base‑7
 *            representations * of 1,2,3,… and interpreting that base‑7 number
 *            in base 10
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t collected = 0;

    for (idx_t i = 1; collected < n; ++i)
    {
        /* Convert i to base 7, store the digits in reverse order. */
        idx_t x = i;
        idx_t digits[32];
        idx_t len = 0;
        do
        {
            digits[len++] = x % 7;
            x /= 7;
        } while (x);

        /* Append the digits in correct order, up to n digits. */
        for (idx_t j = len; j > 0 && collected < n; --j)
        {
            result = result * 7 + (seq_t)digits[j - 1];
            ++collected;
        }
    }

    return result;
}
