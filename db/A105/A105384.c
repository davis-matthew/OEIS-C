/*
 * A105384  Expansion of x/(1 + x + x^2 + x^3 + x^4).
 * https://oeis.org/A105384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 if n%5==1, -1 if n%5==2, else 0
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t cycle[5] = {0, 1, -1, 0, 0};
    return cycle[n % 5];
}
