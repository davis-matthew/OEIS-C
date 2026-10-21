/*
 * A057084  Scaled Chebyshev U-polynomials evaluated at sqrt(2).
 * https://oeis.org/A057084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(a(n-1)-a(n-2)), a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prev1 = (seq_t)1; /* a(0) */
    seq_t prev2 = (seq_t)0; /* a(-1) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t cur = ((seq_t)8) * (prev1 - prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
