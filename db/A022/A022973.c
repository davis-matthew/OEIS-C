/*
 * A022973  a(n) = 17-n.
 * https://oeis.org/A022973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 17 - n
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A022973: a(n) = 17 - n, indexed from 0. */
seq_t a(idx_t n)
{
    return (seq_t)17 - (seq_t)n;
}
