/*
 * A100236  Main diagonal of triangle A100235.
 * https://oeis.org/A100236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + a(n-2) + 5 for n>1, a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 4;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t curr = 5 * prev1 + prev2 + 5;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
