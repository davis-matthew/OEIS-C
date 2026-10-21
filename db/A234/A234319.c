/*
 * A234319  Smallest sum of n-th powers of k+1 consecutive positive integers
 *          that equals the sum of n-th powers of the next k consecutive
 *          integers, or -n if none.
 * https://oeis.org/A234319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0, a(1) = 3, a(2) = 25; for all n ≥ 3, a(n) = -n (no
 *            solution exists for n > 2) The function returns the smallest sum
 *            of n‑th powers that satisfies the equation or –n if no solution
 *            exists
 * Range:     i8 n <= 54, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   return (seq_t)0;
    if (n == 1)   return (seq_t)3;
    if (n == 2)   return (seq_t)25;
    return -(seq_t)n;
}
