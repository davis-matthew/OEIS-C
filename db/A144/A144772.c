/*
 * A144772  Duplicate of A045756.
 * https://oeis.org/A144772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = Π_{k=1..n} (9k-8)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        result *= (seq_t)(9 * k - 8);
    }
    return result;
}
