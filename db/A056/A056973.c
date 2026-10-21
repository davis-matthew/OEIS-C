/*
 * A056973  Number of blocks of {0,0} in the binary expansion of n.
 * https://oeis.org/A056973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0; a(2*n)=a(n)+[(n%2==0)?1:0]; a(2*n+1)=a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n > 1)
    {
        if ((n & 1) == 0)          /* n even : n = 2*m */
        {
            idx_t m = n >> 1;
            if ((m & 1) == 0)      /* m even */
                res += (seq_t)1;
            n = m;
        }
        else                       /* n odd : n = 2*m + 1 */
        {
            n >>= 1;
        }
    }
    return res;
}
