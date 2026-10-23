/*
 * A174971  Periodic sequence: repeat (3,-3).
 * https://oeis.org/A174971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*(-1)^n
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n & 1) ? (seq_t)-3 : (seq_t)3;
}
