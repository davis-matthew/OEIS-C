/*
 * A064385  a(n) = 2*5^n - 3.
 * https://oeis.org/A064385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*5^n - 3. * Offset: 1 (a(1) = 7)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* Compute 2*5^n - 3.  power5(n) returns 5^n as seq_t. */
    seq_t pow = power5(n);
    return (seq_t)2 * pow - (seq_t)3;
}
