/*
 * A084133  Duplicate of A005667.
 * https://oeis.org/A084133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 6*a(n-1) + a(n-2), a(0)=1, a(1)=3
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)3;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)3; /* a(1) */
    seq_t cur = 0;
    idx_t i = 2;

    for (; i <= n; ++i) {
        cur = prev1 * (seq_t)6 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
