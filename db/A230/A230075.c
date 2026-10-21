/*
 * A230075  Period 8: repeat [2, 1, 0, 1, -2, -1, 0, -1].
 * https://oeis.org/A230075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [2,1,0,1,-2,-1,0,-1][n mod 8] with offset 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[8] = {2, 1, 0, 1, -2, -1, 0, -1};
    return table[n & 7];
}
