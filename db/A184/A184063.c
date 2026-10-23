/*
 * A184063  Number of (n+1) X 2 binary arrays with rows and columns in
 *          nondecreasing order and with no 2 X 2 subblock sum differing from
 *          a horizontal or vertical neighbor subblock sum by more than one.
 * https://oeis.org/A184063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 6*n^2 + 5*n + 24)/6 for n > 1; a(1) = 7 (offset =
 *            1)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below offset is irrelevant */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)7;

    seq_t sn = (seq_t)n;
    seq_t term = sn * sn * sn
               + (seq_t)6 * sn * sn
               + (seq_t)5 * sn
               + (seq_t)24;
    term /= (seq_t)6;
    return term;
}
