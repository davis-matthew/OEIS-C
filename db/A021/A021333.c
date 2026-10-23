/*
 * A021333  Decimal expansion of 1/329.
 * https://oeis.org/A021333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/329⌋ mod 10 (decimal digit of 1/329)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the nth decimal digit using long division */
    idx_t remainder = 1;               /* remainder of 1/329 before any digits */
    for (idx_t i = 0; i < n; ++i) {
        remainder = (remainder * 10) % 329;   /* discard one digit */
    }
    idx_t digit = (remainder * 10) / 329;      /* the nth digit */
    return (seq_t)digit;
}
