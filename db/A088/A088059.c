/*
 * A088059  Duplicate of A028387.
 * https://oeis.org/A088059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(n + 1) - 1
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;               /* Convert the index first */
    return sn * (sn + 1) - 1;          /* Compute n*(n+1)-1 using seq_t arithmetic */
}
