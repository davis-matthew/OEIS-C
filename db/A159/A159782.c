/*
 * A159782  a(0)=1; a(1)=a(2)=a(3)=a(4)=2; a(2n+1)=0 for n >= 2; a(4n)=a(4n-2)
 *          = a(n) + a(n+1) for n >= 2.
 * https://oeis.org/A159782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(1)=a(2)=a(3)=a(4)=2; a(2n+1)=0 for n>=2;
 *            a(4n)=a(4n-2)=a(n)+a(n+1) for n>=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n <= 4)
        return (seq_t)2;
    if (n & 1)
        return 0;                /* all odd n >=5 are zero */

    if ((n & 3) == 2)            /* n = 4k-2 */
    {
        idx_t k = (n >> 2) + 1;  /* k = (n+2)/4 without overflow */
        return a(k) + a(k + 1);
    }
    else                         /* n = 4k */
    {
        idx_t k = n >> 2;        /* k = n/4 */
        return a(k) + a(k + 1);
    }
}
