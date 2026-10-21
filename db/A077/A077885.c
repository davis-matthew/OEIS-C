/*
 * A077885  Expansion of (1-x)^(-1)/(1-2*x^3).
 * https://oeis.org/A077885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^{⌊n/3⌋+1} - 1 (sum_{j=0}^{⌊n/3⌋} 2^j)
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of terms in the geometric series */
    idx_t k = n / 3;
    seq_t val = (seq_t)1;
    for (idx_t i = 0; i <= k; ++i)
    {
        val *= (seq_t)2;
    }
    val -= (seq_t)1;
    return val;
}
