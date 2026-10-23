/*
 * A014032  Inverse of 23rd cyclotomic polynomial.
 * https://oeis.org/A014032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n ≡ 0 (mod 23), a(n) = –1 if n ≡ 1 (mod 23),
 *            otherwise 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 23;
    if (r == 0) {
        return (seq_t)1;
    } else if (r == 1) {
        return (seq_t)-1;
    } else {
        return (seq_t)0;
    }
}
