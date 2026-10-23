/*
 * A145287  Duplicate of A016861.
 * https://oeis.org/A145287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 5*n + 1
 * Range:     i8 n <= 25, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)5 * (seq_t)n + 1;
}
