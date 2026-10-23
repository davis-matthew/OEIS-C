/*
 * A021321  Decimal expansion of 1/317.
 * https://oeis.org/A021321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of 1/317 after the decimal point,
 *            with a(0) = first digit after the decimal point (i.e. a(0)=0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 32‑bit unsigned integer for the remainder: it always stays < 317. */
    uint32_t rem = 1;          /* Initial remainder is 1 (numerator). */
    seq_t digit = 0;           /* To store the digit returned. */

    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * 10;                     /* Bring down the next zero. */
        digit = (seq_t)(rem / 317);         /* Next decimal digit. */
        rem = rem % 317;                    /* New remainder. */
    }
    return digit;
}
