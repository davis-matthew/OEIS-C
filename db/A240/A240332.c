/*
 * A240332  Inverse of 49th cyclotomic polynomial.
 * https://oeis.org/A240332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 49 = 0; a(n) = -1 if n mod 49 = 7; otherwise 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 49;
    if (r == 0)
        return 1;
    if (r == 7)
        return -1;
    return 0;
}
