/*
 * A185950  a(n) = 4*n^2 - n - 1.
 * https://oeis.org/A185950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n^2 - n - 1
 * Range:     i8 n <= 5, i16 n <= 90, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;      /* cast index to seq_t to avoid overflow in integer intermediate */
    return 4 * sn * sn - sn - 1;
}
