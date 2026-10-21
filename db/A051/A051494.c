/*
 * A051494  Expansion of (1 - x + x^2 + x^3)/(1 - x^2)^3.
 * https://oeis.org/A051494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2 + 2*n + 2 + (n^2 + 6*n + 6)*(-1)^n)/8
 * Range:     i8 n <= 5, i16 n <= 265, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = nn * nn + (seq_t)2 * nn + (seq_t)2;
    seq_t term2 = nn * nn + (seq_t)6 * nn + (seq_t)6;
    seq_t sign  = ((n & (idx_t)1) ? (seq_t)(-1) : (seq_t)1);
    seq_t num   = term1 + term2 * sign;
    return num / (seq_t)8;
}
