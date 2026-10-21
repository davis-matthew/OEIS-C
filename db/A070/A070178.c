/*
 * A070178  Coefficients of Lehmer's polynomial.
 * https://oeis.org/A070178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, fini, full
 * Formula:   a(n) = coefficient of Lehmer's polynomial * 1 + x - x^3 - x^4 -
 *            x^5 - x^6 - x^7 + x^9 + x^10 * a(0) = 1, a(1) = 1, a(2) = 0,
 *            a(3) = -1, a(4) = -1, a(5) = -1, * a(6) = -1, a(7) = -1, a(8) =
 *            0, a(9) = 1, a(10) = 1, * a(n) = 0 for all n > 10
 * Range:     i8 n <= 10, i16 n <= 10, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n >= 3 && n <= 7) return (seq_t)-1;
    if (n == 8) return (seq_t)0;
    if (n == 9) return (seq_t)1;
    if (n == 10) return (seq_t)1;
    return (seq_t)0;
}
