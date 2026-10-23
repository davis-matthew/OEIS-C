/*
 * A024008  a(n) = 1 - n^10.
 * https://oeis.org/A024008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 78, i128 n <= 360,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t power = (seq_t)1;
    for (int i = 0; i < 10; ++i)
        power *= base;
    return ((seq_t)1) - power;
}
