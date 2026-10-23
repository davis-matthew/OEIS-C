/*
 * A017261  a(n) = (9*n + 8)^5.
 * https://oeis.org/A017261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^5 (offset 0)
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 791, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the base value 9*n + 8 */
    seq_t t = (seq_t)9 * n + 8;

    /* Raise to the 5th power by repeated multiplication */
    seq_t res = t;
    for (int i = 1; i < 5; ++i) {
        res *= t;
    }

    return res;
}
