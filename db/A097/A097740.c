/*
 * A097740  Chebyshev U(n,x) polynomial evaluated at x=201.
 * https://oeis.org/A097740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*201*a(n-1) - a(n-2), with a(0)=1, a(1)=402
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)402;

    /* Two previous terms in the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)402; /* a(1) */
    const seq_t coeff = (seq_t)402; /* 2*201 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = coeff * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
