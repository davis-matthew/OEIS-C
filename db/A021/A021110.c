/*
 * A021110  Decimal expansion of 1/106.
 * https://oeis.org/A021110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n-th digit after the decimal point in the decimal *
 *            expansion of 1/106. a(0) gives the first decimal digit 0, a(1)
 *            the * second decimal digit 0, a(2) the third decimal digit 9,
 *            etc. * The digit is obtained by simulating long division of 1 by
 *            106
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;          /* remainder modulo 106 */
    seq_t digit = 0;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        rem *= 10;                        /* bring down the next digit */
        digit = (seq_t)(rem / 106);       /* integer part is the desired digit */
        rem  %= 106;                      /* remainder for the next step */
    }
    return digit;
}
