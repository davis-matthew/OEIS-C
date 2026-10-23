/*
 * A115299  Greatest digit of n + least digit of n. Different from A088133.
 * https://oeis.org/A115299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = greatest digit of n + least digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_min_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/max_min_digit_sum.h"

seq_t a(idx_t n)
{
    /* Sequence is undefined for n == 0, but offset is 1 */
    return max_min_digit_sum(n);
}
