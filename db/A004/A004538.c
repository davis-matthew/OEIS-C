/*
 * A004538  a(n) = 3*n^2 + 3*n - 1.
 * https://oeis.org/A004538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n^2 + 3*n - 1
 * Range:     i8 n <= 6, i16 n <= 104, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return 3 * n_seq * n_seq + 3 * n_seq - 1;
}
