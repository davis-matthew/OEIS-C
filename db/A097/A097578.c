/*
 * A097578  a(n) = (2*n+1)*2^floor((n+1)/2).
 * https://oeis.org/A097578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*2^floor((n+1)/2)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 50, u64 n <= 112, u128 n <= 238,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor((n+1)/2) */
    idx_t k = (n + 1) >> 1;

    /* Compute 2^k safely (no UB for arbitrary shift amounts) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2 <<= 1;

    /* (2*n+1) */
    seq_t factor = ((seq_t)n * 2) + 1;

    return factor * pow2;
}
