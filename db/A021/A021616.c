/*
 * A021616  Decimal expansion of 1/612.
 * https://oeis.org/A021616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^{n+1} / 612) mod 10, i.e. the nth digit after
 *            the decimal point of 1/612
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t remainder = 1;                 /* remainder before first digit */
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t digit = (remainder * 10) / 612;
        remainder = (remainder * 10) % 612;
        if (i == n)
            return (seq_t)digit;
    }
    /* should not happen */
    return 0;
}
