/*
 * A014226  Inverse of 217th cyclotomic polynomial.
 * https://oeis.org/A014226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n mod 217 in [0,6], -1 for n mod 217 in [31,37], 0
 *            otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % 217u;
    if (r <= 6u) return (seq_t)1;
    if (r >= 31u && r <= 37u) return (seq_t)-1;
    return 0;
}
