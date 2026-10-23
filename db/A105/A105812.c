/*
 * A105812  Expansion of (1+x-x^2)/(1+x).
 * https://oeis.org/A105812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = 0, a(n) = -(-1)^n for n ≥ 2
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    /* (-1)^n is 1 when n is even, -1 when n is odd */
    /* So -(-1)^n is -1 for even n, +1 for odd n */
    return (n & 1) ? 1 : -1;
}
