/*
 * A131533  Period 6: repeat [0, 0, 0, 0, 1, -1].
 * https://oeis.org/A131533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = {0,0,0,0,1,-1}[n mod 6]
 * Range:     i8 n <= 103, i16 n <= 103, i32 n <= 103, i64 n <= 103,
 *            i128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t tbl[6] = {0, 0, 0, 0, 1, -1};
    return tbl[(unsigned)(n % 6)];
}
