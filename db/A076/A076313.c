/*
 * A076313  a(n) = floor(n/10) - (n mod 10).
 * https://oeis.org/A076313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n/10) - (n mod 10)
 * Range:     i8 n <= 1279, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the integer quotient and remainder of n divided by 10. */
    idx_t q = n / 10;    /* floor(n/10)  */
    idx_t d = n % 10;    /* n mod 10     */

    /* Convert to seq_t and perform the subtraction. */
    return (seq_t)q - (seq_t)d;
}
