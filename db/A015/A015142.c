/*
 * A015142  Inverse of 1133rd cyclotomic polynomial.
 * https://oeis.org/A015142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1133 < 11, otherwise 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n % 1133;
    if (m < 11)
        return (seq_t)1;
    return (seq_t)0;
}
