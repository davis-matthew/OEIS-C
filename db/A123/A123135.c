/*
 * A123135  a(n) = n^3 plus sum of digits of n^3.
 * https://oeis.org/A123135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^3 + sum_of_digits(n^3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    return cube + digit_sum(cube);
}
