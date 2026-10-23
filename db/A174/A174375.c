/*
 * A174375  a(n) = n^2 - XOR(n^2, n).
 * https://oeis.org/A174375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, look
 * Formula:   a(n) = n^2 - XOR(n^2, n)
 * Range:     i8 n <= 129, i16 n <= 8192, i32 n <= 8192, i64 n <= 8192,
 *            i128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;          /* n^2 */
    return sq - (sq ^ sn);       /* n^2 - (n^2 XOR n) */
}
