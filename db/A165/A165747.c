/*
 * A165747  a(n) = 1-2n.
 * https://oeis.org/A165747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1-2n
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)1 - ((seq_t)n * (seq_t)2);
}
