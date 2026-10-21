/*
 * A234045  Period 7: repeat [0, 0, 1, -1, -1, 1, 0].
 * https://oeis.org/A234045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [0,0,1,-1,-1,1,0] repeated every 7 terms
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[7] = {0, 0, 1, -1, -1, 1, 0};
    return pattern[n % 7];
}
