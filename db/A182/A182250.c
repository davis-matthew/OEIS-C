/*
 * A182250  a(0)=2, a(n) = (a(n-1) * n) OR n.
 * https://oeis.org/A182250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0)=2; a(n) = (a(n-1)*n) OR n
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = (res * (seq_t)i) | (seq_t)i;
    }
    return res;
}
