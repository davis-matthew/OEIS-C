/*
 * A108773  Concatenation of n and the sum of the digits of n.
 * https://oeis.org/A108773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n * 10^{num_digits(sum_of_digits(n))} + sum_of_digits(n)
 * Range:     u8 n <= 18, u16 n <= 655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the digit sum of n */
    uint64_t digit_sum = 0;
    idx_t temp = n;
    while (temp > 0) {
        digit_sum += temp % 10;
        temp /= 10;
    }

    /* compute 10^{num_digits(digit_sum)} */
    uint64_t sq = digit_sum;      /* keep a copy for later use */
    seq_t pow10 = 10;             /* at least one digit for 0 */
    while (sq >= 10) {
        pow10 *= 10;
        sq   /= 10;
    }

    /* concatenate: n followed by its digit sum */
    return (seq_t)n * pow10 + (seq_t)digit_sum;
}
