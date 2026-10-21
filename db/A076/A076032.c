/*
 * A076032  Duplicate of A004277.
 * https://oeis.org/A076032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 1, a(n) = 2*n - 2 for n >= 2
 * Range:     i8 n <= 64, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t)1;
    }
    return (seq_t)n * 2 - 2;
}
