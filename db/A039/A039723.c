/*
 * A039723  Numbers in base -10.
 * https://oeis.org/A039723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = interpret the base -10 representation of n as a decimal
 *            integer
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long cur = n;      /* signed enough for n up to 10,000 */
    seq_t  res   = 0;
    seq_t  pow10 = 1;

    while (cur != 0)
    {
        long long q = cur / -10;
        int r = cur % -10;          /* r will be in the range [-9,0] */
        if (r < 0)
        {
            r += 10;                /* make remainder positive */
            q += 1;                 /* adjust quotient */
        }
        cur = q;
        res += (seq_t)r * pow10;    /* accumulate digit weighted by 10^k */
        pow10 *= 10;
    }
    return res;                     /* a(0) correctly returns 0 */
}
