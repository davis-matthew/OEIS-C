/*
 * A003058  Duplicate of A000194.
 * https://oeis.org/A003058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = smallest k such that k*(k+1) >= n
 * Range:     i8 n <= 34, i16 n <= 34, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t k = (seq_t)1;
    while (k * (k + 1) < (seq_t)n) {
        k++;
    }
    return k;
}
