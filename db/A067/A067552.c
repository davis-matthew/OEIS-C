/*
 * A067552  a(n) = SumOfDigits(n)^2 - SumOfDigits(n^2), where SumOfDigits =
 *          A007953.
 * https://oeis.org/A067552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = SumOfDigits(n)^2 - SumOfDigits(n^2)
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum_square.h"

/* Sum of decimal digits of a uint64_t.  */
static inline seq_t digit_sum_uint64(idx_t n)
{
    seq_t sum = 0;
    while (n > 0)
    {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t ds   = digit_sum_uint64(n);
    seq_t prod = ds * ds;
    seq_t sqsum = digit_sum_square(n);
    return prod - sqsum;
}
