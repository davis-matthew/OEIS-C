/*
 * A066563  Smallest Catalan number (A000108) divisible by n.
 * https://oeis.org/A066563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest Catalan number divisible by n
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*   C₀ = 1   */
    seq_t catal = (seq_t)1;

    /* check the initial value */
    if (catal % (seq_t)n == 0)
        return catal;

    /* iterate over m = 0,1,2,… – C₀ already tested, C₁ will be computed in the loop */
    for (idx_t m = 0; ; ++m)
    {
        /* recurrence: C_{m+1} = C_m * 2*(2m+1) / (m+2)   */
        idx_t numerator   = 2 * (2 * m + 1);
        idx_t denominator = m + 2;

        catal = (catal * (seq_t)numerator) / (seq_t)denominator;

        if (catal % (seq_t)n == 0)
            return catal;
    }
}
