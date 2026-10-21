/*
 * A076309  a(n) = floor(n/10) - 2*(n mod 10).
 * https://oeis.org/A076309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = floor(n/10) - 2*(n mod 10)
 * Range:     i8 n <= 1279, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute quotient and remainder with the full index type, then cast to seq_t */
    seq_t q = (seq_t)(n / 10);          /* floor(n/10) */
    seq_t r = (seq_t)(n % 10);          /* n mod 10 */
    return q - 2 * r;
}
