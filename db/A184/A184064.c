/*
 * A184064  Number of (n+1) X 3 binary arrays with rows and columns in
 *          nondecreasing order and with no 2 X 2 subblock sum differing from
 *          a horizontal or vertical neighbor subblock sum by more than one.
 * https://oeis.org/A184064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184064: Number of (n+1) X 3 binary arrays with rows and columns in
   nondecreasing order and with no 2 X 2 subblock sum differing from a
   horizontal or vertical neighbor subblock sum by more than one.
   Formula (valid for n>3):
        a(n) = (4*n^3 - 15*n^2 + 74*n - 93) / 3
   Initial terms:
        a(1)=11, a(2)=16, a(3)=36 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n <= 3) {
        static const seq_t first[3] = {11, 16, 36};
        return first[n - 1];
    }

    /* Compute the cubic formula for n >= 4 */
    seq_t N = (seq_t)n;
    seq_t num = (seq_t)4 * N * N * N
              - (seq_t)15 * N * N
              + (seq_t)74 * N
              - 93ULL;
    return num / 3;
}
