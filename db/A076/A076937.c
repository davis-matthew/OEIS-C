/*
 * A076937  Duplicate of A014682.
 * https://oeis.org/A076937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n odd) ? (n-1)/2 : (3*n/2)-1
 * Range:     i8 n <= 28, i16 n <= 28, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 1ULL) == 1) {
        /* n is odd: a(n) = (n-1)/2 */
        return (seq_t)((n - 1) / 2);
    } else {
        /* n is even: a(n) = 3*(n/2)-1 */
        seq_t k = (seq_t)(n / 2);
        return (seq_t)(3 * k - 1);
    }
}
