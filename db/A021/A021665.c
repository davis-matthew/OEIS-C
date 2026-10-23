/*
 * A021665  Decimal expansion of 1/661.
 * https://oeis.org/A021665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit (0‑indexed) of the decimal expansion of
 *            1/661
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  remainder never exceeds 6610, so a 16‑bit integer is sufficient.  */
    uint16_t rem = 1;           /* remainder after each division step   */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;                 /* bring the next digit into the limb  */
        int digit = rem / 661;     /* integer part of the long division   */
        if (i == n)
            return (seq_t)digit;   /* desired digit                          */
        rem -= digit * 661;        /* update remainder: rem %= 661          */
    }
    return (seq_t)0;  /* unreachable, keeps compiler happy      */
}
