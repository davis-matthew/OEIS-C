/*
 * A010891  Inverse of 5th cyclotomic polynomial.
 * https://oeis.org/A010891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n ≡ 0 (mod 5), -1 if n ≡ 1 (mod 5), else 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n % 5) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)-1;
        default: return (seq_t)0;
    }
}
