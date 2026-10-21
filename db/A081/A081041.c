/*
 * A081041  6th binomial transform of (1,5,0,0,0,0,0,0,...).
 * https://oeis.org/A081041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 6) * 6^(n-1) for n >= 1 a(0) = 1 This implements
 *            the direct closed‑form expression for OEIS A081041
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 6^(n-1).  The loop runs n-1 times. */
    seq_t pw = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pw *= (seq_t)6;

    /* coeff = (5*n + 6) */
    seq_t coeff = (seq_t)5 * (seq_t)n + (seq_t)6;

    return coeff * pw;
}
