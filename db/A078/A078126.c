/*
 * A078126  Negative determinant of n X n matrix M_{i,j}=1 if i=j or i+j=1
 *          (mod 2).
 * https://oeis.org/A078126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = floor((n+2)(n-2)/4) = floor((n*n - 4)/4)
 * Range:     i8 n <= 11, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   floor_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_div.h"

seq_t a(idx_t n)
{
    /* compute a(n) according to the closed‑form formula */
    seq_t sq = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t num = sq - 4;                    /* n^2 - 4 */
    return floor_div(num, 4);
}
