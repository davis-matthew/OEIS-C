/*
 * A210626  Values of the prime-generating polynomial 4*n^2 - 284*n + 3449.
 * https://oeis.org/A210626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n^2 - 284*n + 3449
 * Range:     i8 none, i16 n <= 128, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long nn = (long long)n;
    long long val = 4LL * nn * nn - 284LL * nn + 3449LL;
    return (seq_t)val;
}
