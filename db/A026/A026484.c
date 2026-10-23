/*
 * A026484  Erroneous version of A026488.
 * https://oeis.org/A026484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n for n <= 5, * a(n) = 4*n - 15 for n >= 6
 * Range:     i8 n <= 30, i16 n <= 30, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5)
        return (seq_t)n;
    return (seq_t)n * 4 - (seq_t)15;
}
