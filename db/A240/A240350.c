/*
 * A240350  Inverse of 61st cyclotomic polynomial.
 * https://oeis.org/A240350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 61 = 0, -1 if n mod 61 = 1, otherwise 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 61;
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)-1;
        default: return (seq_t)0;
    }
}
