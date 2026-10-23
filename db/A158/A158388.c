/*
 * A158388  -1 followed by infinitely many 1's.
 * https://oeis.org/A158388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = -1 if n==0 else 1
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n == 0) ? (seq_t)-1 : (seq_t)1;
}
