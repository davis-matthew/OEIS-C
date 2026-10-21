/*
 * A054091  Row sums of A054090.
 * https://oeis.org/A054091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1)+2, with a(0)=1, a(1)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)2;          /* a(1) */
    seq_t cur    = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = ((seq_t)(i - 1)) * prev1 + (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
