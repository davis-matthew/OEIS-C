/*
 * A196414  a(n) = n! - n^7.
 * https://oeis.org/A196414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less, easy
 * Formula:   a(n) = n! - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;
    seq_t pow = 1;
    for (int i = 0; i < 7; ++i)
        pow *= (seq_t)n;
    return fact - pow;
}
