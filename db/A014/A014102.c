/*
 * A014102  Inverse of 93rd cyclotomic polynomial.
 * https://oeis.org/A014102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n ≡ 0,1,2 (mod 93); a(n) = -1 for n ≡ 31,32,33 (mod
 *            93); otherwise 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 93;
    if (r < 3)      return (seq_t)1;
    if (r >= 31 && r <= 33) return (seq_t)(-1);
    return (seq_t)0;
}
