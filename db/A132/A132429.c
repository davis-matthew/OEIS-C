/*
 * A132429  Period 4: repeat [3, 1, -1, -3].
 * https://oeis.org/A132429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3 - 2*(n mod 4)
 * Range:     i8 n <= 101, i16 n <= 101, i32 n <= 101, i64 n <= 101,
 *            i128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n modulo 4; works for any power-of-two modulus */
    idx_t mod4 = n & 3ULL;
    seq_t m = (seq_t)mod4;
    return (seq_t)(3 - 2 * m);
}
