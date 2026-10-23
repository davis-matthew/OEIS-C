/*
 * A015450  Inverse of 1441st cyclotomic polynomial.
 * https://oeis.org/A015450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n % 1441 <= 10) ? 1 : 0 (period 1441, 11 consecutive
 *            ones)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t period = 1441;
    static const idx_t last_one = 10;   /* indices 0 .. 10 inclusive are 1 */
    idx_t r = n % period;
    return (r <= last_one) ? (seq_t)1 : (seq_t)0;
}
