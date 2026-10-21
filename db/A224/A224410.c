/*
 * A224410  Number of 3 X n 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 15*n^5 + 145*n^4 + 585*n^3 + 4174*n^2 + 840*n) /
 *            720
 * Range:     u8 none, u16 n <= 2, u32 n <= 50, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via integer arithmetic in seq_t (any unsigned integer type) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n3 * tn;
    seq_t n5 = n4 * tn;
    seq_t n6 = n5 * tn;

    seq_t numerator =
        n6
      + (seq_t)15 * n5
      + (seq_t)145 * n4
      + (seq_t)585 * n3
      + (seq_t)4174 * n2
      + (seq_t)840 * tn;

    return numerator / (seq_t)720;
}
