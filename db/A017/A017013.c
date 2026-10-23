/*
 * A017013  a(n) = (7*n + 2)^9.
 * https://oeis.org/A017013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 19, u128 n <= 2730,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base = 7*n + 2, computed in seq_t */
    seq_t base = (seq_t)7 * (seq_t)n + (seq_t)2;

    /* compute base^9 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 9; ++i)
        result *= base;

    return result;
}
