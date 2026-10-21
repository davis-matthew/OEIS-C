/*
 * A080462  Define f(k) = floor(k/ sum of the digits of k). Let f(f(...(n))) =
 *          m where m is divisible by the sum of the digits of m. Then a(n)=
 *          one more than the least number of steps to obtain m.
 * https://oeis.org/A080462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 + the minimum number of times f(k) = ⌊k /
 *            sum_of_digits(k)⌋ has to be applied to n until the result is
 *            divisible by its digit sum
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    int steps = 0;
    while (1) {
        seq_t s = digit_sum(x);          /* sum of the decimal digits of x (s >= 1 for x >= 1) */
        if (x % s == 0)                  /* condition satisfied */
            break;
        x = x / s;                       /* apply f(k) */
        ++steps;
    }
    return (seq_t)(steps + 1);
}
