/*
 * A105368  Expansion of (1-x-x^3+x^4)/(1-x^5).
 * https://oeis.org/A105368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 - ((n^2 + n + 1) mod 5) Period 5 sequence: [1, -1, 0,
 *            -1, 1]
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[5] = { 1, -1, 0, -1, 1 };
    return pattern[n % 5];
}
