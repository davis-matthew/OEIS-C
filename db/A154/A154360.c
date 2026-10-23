/*
 * A154360  a(n) = 250*n - 180.
 * https://oeis.org/A154360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 250*n - 180
 * Range:     i8 none, i16 n <= 131, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)250 * (seq_t)n;
    result -= (seq_t)180;
    return result;
}
