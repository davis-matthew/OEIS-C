/*
 * A170931  Extended Lucas L(n,i) = n*(L(n,i-1) + L(n,i-2)) = a^i + b^i where
 *          d = sqrt(n*(n+4)); a=(n+d)/2; b=(n-d)/2.
 * https://oeis.org/A170931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 4, a(n) = 4*a(n-1) + 4*a(n-2) (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)4 * prev1 + (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
