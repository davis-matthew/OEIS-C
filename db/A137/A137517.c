/*
 * A137517  a(0) = 121; for n>0, a(n) = a(n-1) - n + 1.
 * https://oeis.org/A137517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (242 - n^2 + n) / 2
 * Range:     i8 none, i16 n <= 182, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)242 - nn * nn + nn;
    return val / 2;
}
