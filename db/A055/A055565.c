/*
 * A055565  Sum of digits of n^4.
 * https://oeis.org/A055565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_of_digits(n^4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 1503, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute n^4 using seq_t arithmetic */
    seq_t x = (seq_t)n;
    x = x * x;   /* n^2 */
    x = x * x;   /* n^4 */

    /* Sum decimal digits */
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}
