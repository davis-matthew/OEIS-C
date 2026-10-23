/*
 * A123344  Expansion of (1+3*x)/(1+2*x).
 * https://oeis.org/A123344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(n) = (-2)^(n-1) for n > 0
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 1;
    /* multiply by -2 repeatedly n-1 times */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)-2;

    return result;
}
