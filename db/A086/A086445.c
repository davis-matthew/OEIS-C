/*
 * A086445  Partial sums of A005578.
 * https://oeis.org/A086445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - floor((n-1)/2), a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t value = 1;                    /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {   /* compute a(1) .. a(n) */
        seq_t sub = (seq_t)((i - 1) >> 1); /* floor((i-1)/2) */
        value = value * 2 - sub;          /* a(i) = 2*a(i-1) - sub */
    }
    return value;
}
