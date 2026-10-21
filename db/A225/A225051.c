/*
 * A225051  Numbers of the form x^3 + SumOfCubedDigits(x).
 * https://oeis.org/A225051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^3 + Sum_of_cubed_digits(n)
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   sum_of_cubed_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_of_cubed_digits.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t cube = sn * sn * sn;      /* n^3 */
    seq_t sum_digits_cube = sum_of_cubed_digits(n);  /* Sum of cubed digits of n */
    return cube + sum_digits_cube;
}
