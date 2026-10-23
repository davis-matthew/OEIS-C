/*
 * A120689  a(n) = 10*a(n-1) - 16*a(n-2), with a(0) = 0 and a(1) = 3.
 * https://oeis.org/A120689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 16*a(n-2); a(0)=0, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)10 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
