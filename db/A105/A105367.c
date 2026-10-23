/*
 * A105367  Expansion of (1-x^3)/(1-x^5).
 * https://oeis.org/A105367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) is periodic with period 5: 1,0,0,-1,0 for n mod 5 =
 *            0,1,2,3,4
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[5] = {1, 0, 0, -1, 0};
    return period[n % 5];
}
