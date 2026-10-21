/*
 * A086111  Numerator of the mean deviation of a discrete uniform distribution
 *          on n elements.
 * https://oeis.org/A086111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, changed
 * Formula:   a(n) = odd case: let m = (n-1)/2, then a(n) = m*(m+1); even
 *            case: let m = n/2; if m is even (i.e. n divisible by 4) then
 *            a(n) = m/2; else a(n) = m
 * Range:     u8 n <= 32, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (N & (seq_t)1) /* odd n */
    {
        seq_t m = N >> 1;                 /* m = (n-1)/2          */
        return m * (m + (seq_t)1);
    }
    else /* even n */
    {
        seq_t m = N >> 1;                 /* m = n/2              */
        if ((m & (seq_t)1) == 0)          /* m even, n divisible by 4 */
            return m >> 1;                /* return m/2           */
        else
            return m;                     /* return m             */
    }
}
