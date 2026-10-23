/*
 * A049013  Duplicate of A020735.
 * https://oeis.org/A049013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*n + 3
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t and compute the linear formula */
    return (seq_t)n * (seq_t)2 + (seq_t)3;
}
