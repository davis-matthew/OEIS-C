/*
 * A170930  G(n,1) with n index G(n,i)=n*(G(n,i-1)+G(n,i-2))=(a^i-b^i)*d where
 *          d=sqrt(n*(n+4)); a=(n+d)/2; b=(n-d)/2.
 * https://oeis.org/A170930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * a(n-1) + 3 * a(n-2); a(0) = 0; a(1) = 21
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)21;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)21;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* compute next term: a(i) = 3*a(i-1) + 3*a(i-2) */
        cur = (seq_t)3 * prev1 + (seq_t)3 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
