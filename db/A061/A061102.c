/*
 * A061102  Duplicate of A016779.
 * https://oeis.org/A061102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (3*n + 1)^3
 * Range:     i8 n <= 1, i16 n <= 10, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (3*n + 1) in seq_t */
    seq_t t = (seq_t)n * 3 + 1;
    /* return its cube: t * t * t */
    return t * t * t;
}
