/*
 * A053113  Expansion of (-1 + 1/(1-10*x)^10)/(100*x); related to A053109.
 * https://oeis.org/A053113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^(n-1) * binomial(n+10, 9) (a(0)=1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* compute binomial(n+10, 9) iteratively to avoid overflow */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 9; ++i)
    {
        /* multiply by (n + 10 - i + 1) then divide by i */
        binom = binom * (seq_t)(n + 10 - i + 1) / (seq_t)i;
    }

    /* compute 10^(n-1) by repeated multiplication */
    seq_t pow10 = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow10 *= (seq_t)10;

    return pow10 * binom;
}
