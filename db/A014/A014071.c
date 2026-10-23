/*
 * A014071  Inverse of 62nd cyclotomic polynomial.
 * https://oeis.org/A014071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 62 = 0 or 1, a(n) = -1 if n mod 62 = 31 or 32,
 *            otherwise a(n) = 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t m = n % 62ULL;
    if (m == 0 || m == 1)
        return (seq_t)1;
    if (m == 31 || m == 32)
        return (seq_t)(-1);
    return (seq_t)0;
}
