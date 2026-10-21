/*
 * A079588  a(n) = (n+1)*(2*n+1)*(4*n+1).
 * https://oeis.org/A079588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)(2n + 1)(4n + 1)
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn + 1U;          /* (n + 1)   */
    seq_t t2 = (2 * nn) + 1U;    /* (2n + 1)  */
    seq_t t3 = (4 * nn) + 1U;    /* (4n + 1)  */
    return t1 * t2 * t3;
}
