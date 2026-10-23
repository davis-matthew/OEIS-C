/*
 * A133942  a(n) = (-1)^n * n!.
 * https://oeis.org/A133942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * n!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        result = - (seq_t)i * result;   /* a(i) = -i * a(i-1) */
    }
    return result;
}
