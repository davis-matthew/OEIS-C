/*
 * A076311  a(n) = floor(n/10) - 5*(n mod 10).
 * https://oeis.org/A076311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n/10) - 5*(n mod 10)
 * Range:     i8 n <= 1279, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 10;          /* floor(n/10) */
    idx_t r = n % 10;          /* n mod 10 */
    return (seq_t)q - (seq_t)(5 * r);
}
