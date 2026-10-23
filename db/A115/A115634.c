/*
 * A115634  Expansion of (1-4*x^2)/(1-x^2).
 * https://oeis.org/A115634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*0^n - 3*(1 + (-1)^n)/2
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    else if ((n & 1) == 0)
        return (seq_t)-3;
    else
        return (seq_t)0;
}
