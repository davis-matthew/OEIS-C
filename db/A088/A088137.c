/*
 * A088137  Generalized Gaussian Fibonacci integers.
 * https://oeis.org/A088137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2), with a(0)=0, a(1)=1
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 40, i64 n <= 80, i128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = 2 * prev1 - 3 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
