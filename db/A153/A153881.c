/*
 * A153881  1 followed by -1, -1, -1, ... .
 * https://oeis.org/A153881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 1; a(n) = -1 for n > 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    return (seq_t)-1;
}
