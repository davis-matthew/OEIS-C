/*
 * A163302  Duplicate of A084057.
 * https://oeis.org/A163302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2); a(1) = 1, a(2) = 6
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 18, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;

    seq_t prev2 = (seq_t)1; /* a(1) */
    seq_t prev1 = (seq_t)6; /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 2 * prev1 + 4 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
