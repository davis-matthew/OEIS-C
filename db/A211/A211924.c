/*
 * A211924  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal, vertical or antidiagonal neighbor, and containing
 *          the value n(n+1)/2-2.
 * https://oeis.org/A211924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 2 n^3 - 13 n^2 + 18 n)/8 for n > 1, a(1) = 0 (This
 *            follows from the empirical polynomial given in the OEIS entry.)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for completeness, not used directly */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t numerator = x4 + (seq_t)2 * x3 - (seq_t)13 * x2 + (seq_t)18 * x;
    return numerator / (seq_t)8;
}
