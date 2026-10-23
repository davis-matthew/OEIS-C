/*
 * A174748  x-values in the solution to x^2-33*y^2=1.
 * https://oeis.org/A174748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 46*a(n-1) - a(n-2) with a(1)=1, a(2)=23 (OEIS A174748)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)23;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)23;
    seq_t cur   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur   = (seq_t)46 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
