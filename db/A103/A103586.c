/*
 * A103586  a(0)=1, for n > 0: n-th run consists of 2^n-1 copies of n+1.
 * https://oeis.org/A103586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; for n>0, a(n)=r+1 where r is the smallest integer with n
 *            <= 2^(r+1)-2-r (that is, n lies in the r-th run of length 2^r-1
 *            with value r+1)
 * Range:     u8 n <= 1983, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return (seq_t)1;

    /* Use a running power of two: power = 2^(r+1) */
    seq_t power = (seq_t)4;          /* r=1 → power=2^(1+1)=4 */
    idx_t   r     = 1;
    seq_t  nn    = (seq_t)n;        /* cast once for comparisons */

    while (1)
    {
        seq_t cum = power - (seq_t)2 - (seq_t)r;   /* cumulative length up to run r */
        if (nn <= cum)
            return (seq_t)(r + 1);                /* value of the current run */
        power *= (seq_t)2;   /* advance to next run: power = 2^(r+2) */
        ++r;
    }
}
