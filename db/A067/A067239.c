/*
 * A067239  a(0)=1, a(n) = 8*n*(2*n-1).
 * https://oeis.org/A067239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n*(2*n-1), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* Compute using seq_t arithmetic only. */
    seq_t sn = (seq_t)n;
    return sn * 8 * (sn * 2 - 1);
}
