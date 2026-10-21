/*
 * A215898  a(4n) = 1+4n, a(1+4n) = -2-6n, a(2+4n) = 4+6n, a(3+4n) = -3-4n.
 * https://oeis.org/A215898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4n) = 1+4n, a(1+4n) = -2-6n, a(2+4n) = 4+6n, a(3+4n) = -3-4n
 * Range:     i8 n <= 85, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 3;
    if (r == 0) {
        seq_t k = (seq_t)(n / 4);
        return 1 + k * 4;
    } else if (r == 1) {
        seq_t k = (seq_t)((n - 1) / 4);
        return -(2 + k * 6);
    } else if (r == 2) {
        seq_t k = (seq_t)((n - 2) / 4);
        return 4 + k * 6;
    } else { /* r == 3 */
        seq_t k = (seq_t)((n - 3) / 4);
        return -(3 + k * 4);
    }
}
