/*
 * A092220  Expansion of x*(1-x)/ ((1+x)*(1-x+x^2)) in powers of x.
 * https://oeis.org/A092220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = [0, 1, -1, 0, -1, 1][n mod 6] (period 6)
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[6] = {0, 1, -1, 0, -1, 1};
    return period[n % 6];
}
