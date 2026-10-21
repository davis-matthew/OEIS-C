/*
 * A201157  y-values in the solution to 5*x^2 - 20 = y^2.
 * https://oeis.org/A201157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=5; for n>2, a(n)=3*a(n-1)-a(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;
    if (n == (idx_t)2)
        return (seq_t)5;

    seq_t prev2 = (seq_t)0; /* a(1) */
    seq_t prev1 = (seq_t)5; /* a(2) */
    seq_t curr;

    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
