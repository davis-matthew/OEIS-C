/*
 * A216998  Digit sum of n*7 mod 7.
 * https://oeis.org/A216998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(n*7) mod 7
 * Range:     u8 n <= 36, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n * 7;
    seq_t sum  = digit_sum(prod);
    return sum % 7;
}
