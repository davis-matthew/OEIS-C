/*
 * A097141  Expansion of x*(1+2*x)/(1+x)^2.
 * https://oeis.org/A097141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n-2)*(-1)^n for n ≥ 1, a(0) = 0
 * Range:     i8 n <= 129, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Convert the unsigned index to signed arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute (n-2) in signed form */
    seq_t val = nn - 2;

    /* Multiply by (-1)^n: negate if n is odd */
    if (n & 1)
        val = -val;

    return val;
}
