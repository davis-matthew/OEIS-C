/*
 * A165378  Number of slanted n X 4 (i=1..n) X (j=i..4+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 2 neighbo...
 * https://oeis.org/A165378
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2) = 33, a(3) = 94, and for n ≥ 4: a(n) = 9*n^2 + 4*n – 2
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)33;
    if (n == 3) return (seq_t)94;
    seq_t tn = (seq_t)n;
    return (seq_t)9 * tn * tn + (seq_t)4 * tn - (seq_t)2;
}
