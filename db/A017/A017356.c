/*
 * A017356  a(n) = (10*n+7)^4.
 * https://oeis.org/A017356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 24, u64 n <= 6552, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)10 * (seq_t)n) + (seq_t)7;
    seq_t m2 = m * m;
    return m2 * m2;
}
