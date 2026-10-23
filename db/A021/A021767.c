/*
 * A021767  Decimal expansion of 1/763.
 * https://oeis.org/A021767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit (0‑based) of the decimal expansion of 1/763
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Compute the digit with a simple long division step repeated n+1 times. */
    uint64_t remainder = 1;   /* remainder before producing each digit */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10;               /* shift in next decimal digit */
        digit = (seq_t)(remainder / 763);   /* quotient is the next digit */
        remainder = remainder % 763;           /* update remainder */
    }
    return digit;
}
