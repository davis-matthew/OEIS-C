/*
 * A134967  List of quadruples: [-2n-1, 2n+2, -2n-1, 2n+2].
 * https://oeis.org/A134967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*floor(n/4)-1 if n mod 4 ∈ {0,2}; 2*floor(n/4)+2
 *            otherwise
 * Range:     i8 n <= 252, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t q = n / 4;              /* floor(n/4) */
    idx_t r = n & 3;              /* n % 4 */

    if (r == 0 || r == 2) {
        return -((seq_t)2 * (seq_t)q + 1);
    } else {
        return (seq_t)2 * (seq_t)q + 2;
    }
}
