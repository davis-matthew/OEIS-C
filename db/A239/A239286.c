/*
 * A239286  Expansion of (x + 1)*(3*x^2 + 2*x + 1)/(x^2 + x + 1)^2.
 * https://oeis.org/A239286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3n) = 1 - 3n, a(3n+1) = 1, a(3n+2) = 3n
 * Range:     i8 n <= 130, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % (idx_t)3;
    seq_t m = (seq_t)(n / (idx_t)3);
    if (r == (idx_t)1) {
        return (seq_t)1;
    } else if (r == (idx_t)0) {
        return (seq_t)(1) - (seq_t)(3 * m);
    } else { /* r == 2 */
        return (seq_t)(3 * m);
    }
}
