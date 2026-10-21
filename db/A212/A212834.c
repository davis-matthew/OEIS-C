/*
 * A212834  Number of 0..7 arrays of length n+1 with 0 never adjacent to 7.
 * https://oeis.org/A212834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 6*a(n-2) with initial values a(1) = 62, a(2) =
 *            482
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)62;
    if (n == 2) return (seq_t)482;

    seq_t prev2 = (seq_t)62;      /* a(1) */
    seq_t prev1 = (seq_t)482;     /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur   = (seq_t)7 * prev1 + (seq_t)6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
