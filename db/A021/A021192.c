/*
 * A021192  Decimal expansion of 1/188.
 * https://oeis.org/A021192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit after the decimal point in 1/188
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint32_t remainder = 1;   /* remainder of 1 ÷ 188 */
    uint8_t digit = 0;        /* digit to be returned */

    /* The long division algorithm: at each step
       digit = floor(remainder * 10 / 188),
       remainder = (remainder * 10) % 188.      */
    for (idx_t i = 0; i <= n; ++i) {
        uint32_t prod = remainder * 10u;
        digit = (uint8_t)(prod / 188u);
        remainder = prod % 188u;
    }

    return (seq_t)digit;
}
