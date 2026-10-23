/*
 * A021506  Decimal expansion of 1/502.
 * https://oeis.org/A021506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit of the decimal expansion of 1/502
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t rem = 1;          /* remainder before the digit */
    seq_t digit = 0;          /* the digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * 10;
        digit = rem / 502;     /* integer part: the digit */
        rem   = rem % 502;     /* new remainder for next step */
    }

    return digit;
}
