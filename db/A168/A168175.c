/*
 * A168175  Expansion of 1/(1 - 4*x + 7*x^2).
 * https://oeis.org/A168175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) with a(0)=1, a(1)=4
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 22, i64 n <= 44, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    seq_t prev2 = 1;
    seq_t prev1 = 4;
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = 4 * prev1 - 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
