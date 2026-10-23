/*
 * A143248  Erroneous version of A010790.
 * https://oeis.org/A143248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 0 for n=1, else (n!)^2 * (n+1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return fact * fact * (seq_t)(n + 1);
}
