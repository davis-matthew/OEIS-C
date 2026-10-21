/*
 * A070697  Duplicate of A070477.
 * https://oeis.org/A070697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^3 mod 15
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n % 15;      /* reduce modulo 15 first */
    return (m * m * m) % 15;      /* 0 ≤ m ≤ 14, product fits in seq_t */
}
