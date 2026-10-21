/*
 * A093144  Third binomial transform of Pell(3*n)/Pell(3).
 * https://oeis.org/A093144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0; a(1)=1; a(n)=20*a(n-1)-50*a(n-2) for n≥2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur   = (seq_t)0;
    for (idx_t i = 2u; i <= n; ++i)
    {
        cur = prev1 * ((seq_t)20) - prev2 * ((seq_t)50);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
