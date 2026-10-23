/*
 * A021201  Decimal expansion of 1/197.
 * https://oeis.org/A021201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th digit after the decimal point of 1/197. a(0) =
 *            0, a(1) = 0, a(2) = 5,
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t denom = 197;
    seq_t rem = 1;
    seq_t digit = 0;

    /* Perform long division up to the requested digit */
    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * 10;          /* bring down next digit */
        digit = rem / denom;     /* current decimal digit */
        rem = rem % denom;       /* remainder for next step */
    }
    return digit;
}
