/*
 * A240330  Inverse of 43rd cyclotomic polynomial.
 * https://oeis.org/A240330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 43 = 0; -1 if n mod 43 = 1; 0 otherwise
 *            (periodic with period 43)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 43;
    if (r == 0)
        return (seq_t)1;
    else if (r == 1)
        return (seq_t)-1;
    else
        return (seq_t)0;
}
