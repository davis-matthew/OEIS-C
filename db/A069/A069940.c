/*
 * A069940  (1/2)*((Sum of digits of n)^2 + (Sum of digits^2 of n)).
 * https://oeis.org/A069940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (digit_sum(n)^2 + sum_of_squares_of_digits(n)) / 2
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A069940: (1/2) * ( sum_of_digits(n)^2 + sum_of_digits(n)^2 ) */
seq_t a(idx_t n)
{
    seq_t digit_sum = 0;
    seq_t sum_squares = 0;
    idx_t tmp = n;

    while (tmp) {
        unsigned d = tmp % 10;
        digit_sum += d;
        sum_squares += (seq_t)d * d;
        tmp /= 10;
    }

    seq_t sq = digit_sum * digit_sum;
    return (sq + sum_squares) / 2;
}
