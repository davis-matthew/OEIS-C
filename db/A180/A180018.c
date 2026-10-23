/*
 * A180018  Difference of sums of digits of n in decimal and in binary
 *          representation.
 * https://oeis.org/A180018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = sum_of_decimal_digits(n) - popcount(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   decimal_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/decimal_digit_sum.h"

/* Count the number of 1‑bits in the binary representation of n.  */
static seq_t popcount_u64(idx_t n)
{
    seq_t count = 0;
    uint64_t x = n;
    while (x) {
        count++;
        x &= x - 1;   /* remove the lowest set bit */
    }
    return count;
}

/* OEIS A180018: difference of decimal digit sum and binary popcount. */
seq_t a(idx_t n)
{
    return decimal_digit_sum(n) - popcount_u64(n);
}
