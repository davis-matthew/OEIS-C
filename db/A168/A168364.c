/*
 * A168364  a(n) = n^5*(n^2 + 1)/2.
 * https://oeis.org/A168364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 * (n^2 + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 624,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work in seq_t to avoid any overflow from idx_t operations. */
    seq_t tn   = (seq_t)n;            /* n as seq_t */
    seq_t n2   = tn * tn;             /* n^2   */
    seq_t n4   = n2 * n2;             /* n^4   */

    if ((tn & 1) == 0)                 /* n even */
    {
        /* Divide the even factor n by 2 first. */
        seq_t half = tn / 2;
        seq_t prod = half * n4;        /* (n/2) * n^4   */
        return prod * (n2 + 1);        /* (n/2) * n^4 * (n^2 + 1) */
    }
    else                              /* n odd */
    {
        /* (n^2 + 1) is even for odd n; divide by 2 first. */
        seq_t half = (n2 + 1) / 2;
        /* Compute n^5 as tn * n^4 to keep intermediate values small. */
        seq_t n5 = tn * n4;
        return n5 * half;             /* n^5 * ( (n^2 + 1) / 2 ) */
    }
}
