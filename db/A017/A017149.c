/*
 * A017149  Duplicate of A004771.
 * https://oeis.org/A017149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 8*n + 7
 * Range:     i8 n <= 15, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 8 + 7;
}
