/*
 * A121622  Real part of (3 + 2i)^n.
 * https://oeis.org/A121622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2), a(0) = 1, a(1) = 3
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 35, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 3;

    seq_t prev2 = 1;          /* a(0) */
    seq_t prev1 = 3;          /* a(1) */
    seq_t cur  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)6 * prev1 - (seq_t)13 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
