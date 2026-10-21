/*
 * A081269  Duplicate of A064226.
 * https://oeis.org/A081269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (9*n^2 + 13*n + 6) / 2
 * Range:     i8 n <= 3, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the result type first to avoid unwanted precision loss. */
    seq_t x = (seq_t)n;
    seq_t val = (seq_t)9 * x * x + (seq_t)13 * x + (seq_t)6;
    return val / (seq_t)2;
}
