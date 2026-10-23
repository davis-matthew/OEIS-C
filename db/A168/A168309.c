/*
 * A168309  Period 2: repeat 4,-3.
 * https://oeis.org/A168309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - 7*(-1)^n)/2
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & 1) ? (seq_t)4 : (seq_t)-3;
}
