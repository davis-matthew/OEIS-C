/*
 * A017636  a(n) = (12*n + 9)^8.
 * https://oeis.org/A017636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 9)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute base = 12*n + 9 in exact type */
    seq_t base = (seq_t)12 * n + (seq_t)9;
    /* Compute base^8 by repeated multiplication */
    seq_t res = (seq_t)1;
    for (int i = 0; i < 8; ++i)
        res *= base;
    return res;
}
