/*
 * A168330  Period 2: repeat [3, -2].
 * https://oeis.org/A168330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-5*(-1)^n + 1)/2
 * Range:     i8 n <= 84, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sign = (n & 1ULL) ? -1 : 1;   /* (-1)^n */
    return ((seq_t)-5 * sign + 1) / 2;
}
