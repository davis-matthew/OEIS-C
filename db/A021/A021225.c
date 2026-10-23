/*
 * A021225  Decimal expansion of 1/221.
 * https://oeis.org/A021225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of the fraction 1/221 (starting with
 *            a(0)=0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;      /* remainder of the division 1/221 */
    uint64_t digit = 0;    /* current digit, always 0‑9 */

    /* Perform long division up to position n.  After the loop
       the variable 'digit' contains the nth digit. */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                 /* bring down next 0 */
        digit = rem / 221;         /* integer part is next digit */
        rem -= digit * 221;        /* update remainder for next step */
    }
    return (seq_t)digit;          /* emit the found digit */
}
