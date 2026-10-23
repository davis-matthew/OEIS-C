/*
 * A024000  a(n) = 1 - n.
 * https://oeis.org/A024000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n
 * Range:     i8 n <= 129, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use seq_t arithmetic to ensure proper type handling. */
    return (seq_t)1 - (seq_t)n;
}
