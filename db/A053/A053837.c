/*
 * A053837  Sum of digits of n modulo 10.
 * https://oeis.org/A053837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (sum of decimal digits of n) mod 10
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int sum = 0;
    idx_t m = n;
    while (m > 0) {
        sum += m % 10;
        m /= 10;
    }
    return (seq_t)(sum % 10);
}
