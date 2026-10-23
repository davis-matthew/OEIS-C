/*
 * A024007  a(n) = 1 - n^9.
 * https://oeis.org/A024007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 128, i128 n <= 388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t power = 1;
    for (idx_t i = 0; i < 9; ++i) {
        power *= base;
    }
    return (seq_t)1 - power;
}
