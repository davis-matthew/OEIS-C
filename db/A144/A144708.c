/*
 * A144708  a(n) = 6^n * (1-4*n).
 * https://oeis.org/A144708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6^n * (1-4*n)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 9, i64 n <= 21, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= 6;
    }
    seq_t factor = (seq_t)1 - (seq_t)4 * (seq_t)n;
    return pow * factor;
}
