/*
 * A085488  Duplicate of A072265.
 * https://oeis.org/A085488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 2, a(1) = 1, a(n) = a(n-1) + 4*a(n-2) for n >= 2
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev1 + (prev2 << 2); /* prev1 + 4*prev2 */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
