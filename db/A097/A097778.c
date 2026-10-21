/*
 * A097778  Chebyshev polynomials S(n,23) with Diophantine property.
 * https://oeis.org/A097778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=23, a(n)=23*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1U;
    if (n == 1)
        return (seq_t)23U;

    seq_t prev2 = (seq_t)1U;  /* a(0) */
    seq_t prev1 = (seq_t)23U; /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 * (seq_t)23U - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
