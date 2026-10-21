/*
 * A081133  a(n) = n^n*binomial(n+2, 2).
 * https://oeis.org/A081133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n * (n+1) * (n+2) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial(n+2, 2) = (n+1)*(n+2)/2 */
    seq_t binom = ((seq_t)n + (seq_t)1) * ((seq_t)n + (seq_t)2) / (seq_t)2;

    /* Compute n^n by repeated multiplication */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow *= (seq_t)n;
    }

    return pow * binom;
}
