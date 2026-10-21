/*
 * A097690  Numerators of the continued fraction n-1/(n-1/...) [n times].
 * https://oeis.org/A097690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = y(n,n), where for fixed n y(0,n) = 1, y(1,n) = n,
 *            y(k+1,n) = n*y(k,n) - y(k-1,n). The returned value is a(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1; n=0 unused */

    seq_t prev2 = (seq_t)1;        /* y(0,n) */
    seq_t prev1 = (seq_t)n;        /* y(1,n) */

    if (n == 1)
        return prev1;              /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)n - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;                  /* y(n,n) = a(n) */
}
