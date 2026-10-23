/*
 * A024009  a(n) = 1 - n^11.
 * https://oeis.org/A024009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 52, i128 n <= 340,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = (seq_t)1;
    seq_t base = (seq_t)n;
    for (idx_t i = 0; i < 11; i++)
        pow *= base;
    return (seq_t)1 - pow;
}
