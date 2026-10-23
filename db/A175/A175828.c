/*
 * A175828  a(n) = (n*(6*n+1)+(n+2)*(-1)^n)/2.
 * https://oeis.org/A175828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(6*n+1)+(n+2)*(-1)^n)/2
 * Range:     u8 n <= 6, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once for faster repeated use. */
    seq_t sn = (seq_t)n;

    /* Compute n*(6*n+1) safely in seq_t. */
    seq_t t1 = sn * (((seq_t)6 * sn) + 1);

    /* Compute the adjustment term (n+2). */
    seq_t t2 = sn + (seq_t)2;

    /* Use the parity of n to determine (+1) or (-1) for (-1)^n. */
    if ((n & 1) == 0)           /* n is even: (-1)^n = +1 */
        return (t1 + t2) / 2;
    else                        /* n is odd: (-1)^n = -1 */
        return (t1 - t2) / 2;
}
