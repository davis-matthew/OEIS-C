/*
 * A126423  a(n) = n^4 - n - 1.
 * https://oeis.org/A126423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^4 - n - 1
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 215, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;          /* cast index to seq_t */
    seq_t sq = sn * sn;    /* n^2 */
    seq_t n4 = sq * sq;    /* n^4 */
    return n4 - sn - 1;
}
