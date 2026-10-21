/*
 * A072478  Surface area of n-dimensional sphere of radius r is n*V_n*r^(n-1)
 *          = n*Pi^(n/2)*r^(n-1)/(n/2)! = S_n*Pi^floor(n/2)*r^(n-1); sequence
 *          gives numerator of S_n.
 * https://oeis.org/A072478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=0, a(2)=a(4)=2, a(n)=1 for even n>4, a(n)=2^((n+1)/2) for
 *            odd n
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special small even indices */
    if (n == 0)
        return (seq_t)0;
    if (n == 2 || n == 4)
        return (seq_t)2;

    /* odd indices: a(n) = 2^((n+1)/2) */
    if (n & 1U)
    {
        idx_t exp = (n + 1) / 2;   /* exponent of 2 */
        seq_t  res = 1;
        while (exp--)
            res <<= 1;            /* 2 * res */
        return res;
    }

    /* even indices n > 4 */
    return (seq_t)1;
}
