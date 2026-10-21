/*
 * A217928  Sum of distinct decimal digits appearing in n.
 * https://oeis.org/A217928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of distinct decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    uint16_t mask = 0;
    seq_t sum = 0;

    while (m > 0) {
        uint8_t d = m % 10;
        uint16_t bit = 1U << d;
        if ((mask & bit) == 0) {
            mask |= bit;
            sum += d;
        }
        m /= 10;
    }
    return sum;
}
