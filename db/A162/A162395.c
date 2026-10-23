/*
 * A162395  a(n) = -(-1)^n * n^2.
 * https://oeis.org/A162395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = -(-1)^n * n^2
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t to avoid implicit type conversions */
    seq_t sq = (seq_t)n * (seq_t)n;
    /* use the parity of n: odd gives positive, even gives negative */
    return (n & 1) ? sq : -sq;
}
