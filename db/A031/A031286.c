/*
 * A031286  Additive persistence: number of summations of digits needed to
 *          obtain a single digit (the additive digital root).
 * https://oeis.org/A031286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of times the digits of n must be summed until a
 *            single digit is obtained (additive persistence)
 * Range:     u8 n <= 462, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x >= (seq_t)10) {
        seq_t sum = 0;
        seq_t tmp = x;
        while (tmp > (seq_t)0) {
            sum += tmp % (seq_t)10;
            tmp /= (seq_t)10;
        }
        x = sum;
        ++steps;
    }
    return steps;
}
