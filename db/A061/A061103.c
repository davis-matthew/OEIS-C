/*
 * A061103  Duplicate of A016791.
 * https://oeis.org/A061103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (3n-1)^3, n ≥ 1
 * Range:     i8 n <= 2, i16 n <= 10, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 3 - 1;
    seq_t square = base * base;
    return square * base;
}
