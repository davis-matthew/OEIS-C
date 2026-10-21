/*
 * A094055  Duplicate of A094056.
 * https://oeis.org/A094055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n-4)^2 + 5 for n >= 5; a(1) = 1, a(2) = 1, a(3) = 2,
 *            a(4) = 3
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 3;
    /* n >= 5 */
    seq_t k = (seq_t)(n - 4);
    return k * k + 5;
}
