/*
 * A014028  Inverse of 19th cyclotomic polynomial.
 * https://oeis.org/A014028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n % 19 == 0, -1 if n % 19 == 1, 0 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 19U;
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)-1;
    return (seq_t)0;
}
