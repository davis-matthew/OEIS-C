/*
 * A056541  a(n) = 2*n*a(n-1) + 1 with a(0)=0.
 * https://oeis.org/A056541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n-1)+1 with a(0)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;               /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* compute a(i) = 2*i*a(i-1)+1 */
        res = ((seq_t)2 * (seq_t)i) * res + 1;
    }
    return res;
}
