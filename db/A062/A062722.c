/*
 * A062722  a(n) = ceiling(n/3)*round(n/4).
 * https://oeis.org/A062722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/3)*round(n/4)
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* round(n/4) is the nearest integer to n/4, with .5 rounded up. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute ceil(n/3) = n/3 + (n%3 != 0) */
    idx_t ce = n / 3 + ((n % 3) != 0);

    /* Compute round(n/4) = n/4 + (remainder >= 2) */
    idx_t ro = n / 4 + ((n % 4) >= 2 ? 1 : 0);

    return (seq_t)ce * (seq_t)ro;
}
