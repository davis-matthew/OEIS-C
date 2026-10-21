/*
 * A066568  a(n) = n - sum of digits of n.
 * https://oeis.org/A066568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n - sum of digits of n
 * Range:     u8 n <= 268, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Convert index to the sequence's integer type */
    seq_t val = (seq_t)n;

    /* Compute the sum of the decimal digits of val */
    seq_t sum = digit_sum(val);

    /* The OEIS definition is simply the difference */
    return val - sum;
}
