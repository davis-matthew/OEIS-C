/*
 * A078070  Expansion of (1-x)/(1 + 2*x + 2*x^2 + x^3).
 * https://oeis.org/A078070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [1, -3, 4, -3, 1, 0][n mod 6]
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = { 1, -3, 4, -3, 1, 0 };
    return pattern[n % 6];
}
