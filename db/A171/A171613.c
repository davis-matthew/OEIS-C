/*
 * A171613  a(n) = n^2 + sum of the digits of n^2.
 * https://oeis.org/A171613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n^2 + sum_of_digits(n^2)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    seq_t sum = 0;
    seq_t temp = sq;
    while (temp > (seq_t)0) {
        sum += temp % (seq_t)10;
        temp /= (seq_t)10;
    }
    return sq + sum;
}
