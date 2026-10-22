/*
 * A001478  The negative integers.
 * https://oeis.org/A001478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, sign, easy
 * Formula:   a(n) = -n
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return -(seq_t)n;
}
