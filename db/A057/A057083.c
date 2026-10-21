/*
 * A057083  Scaled Chebyshev U-polynomials evaluated at sqrt(3)/2; expansion
 *          of 1/(1 - 3*x + 3*x^2).
 * https://oeis.org/A057083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2), with a(0)=1, a(1)=3
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 37, i64 n <= 78, i128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev1 - prev2) * (seq_t)3;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
