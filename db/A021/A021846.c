/*
 * A021846  Decimal expansion of 1/842.
 * https://oeis.org/A021846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 842) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the nth digit after the decimal point of 1/842. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The remainder in the long division.  It never exceeds 842. */
    uint64_t rem = 1;
    idx_t i;
    seq_t digit = 0;

    for (i = 0; i <= n; ++i)
    {
        rem *= 10;                     /* multiply remainder by 10 */
        digit = (seq_t)(rem / 842);    /* next decimal digit            */
        rem %= 842;                    /* keep only the new remainder   */
    }
    return digit;
}
