/*
 * A160944  a(n)= n * digital sum(n-1) * digital sum(n+1).
 * https://oeis.org/A160944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n * A007953(n-1) * A007953(n+1) * where A007953(x) is the
 *            digital sum of x
 * Range:     u8 n <= 6, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Ensure n is at least 1 (offset 1). */
    if (n < 1) return 0;

    /* Convert index to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;

    /* Compute digital sums of n-1 and n+1. */
    seq_t ds1 = digit_sum(sn - 1);
    seq_t ds2 = digit_sum(sn + 1);

    /* Return the product. */
    return sn * ds1 * ds2;
}
