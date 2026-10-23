/*
 * A128203  Sum of the digits of n*(n+1).
 * https://oeis.org/A128203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Sum_digits(n*(n+1))
 * Range:     u8 n <= 15, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n;
    seq_t prod = m * (m + (seq_t)1);
    return digit_sum(prod);
}
