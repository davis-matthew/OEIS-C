/*
 * A071252  a(n) = n*(n - 1)*(n^2 + 1)/2.
 * https://oeis.org/A071252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(n^2 + 1)/2
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 304, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                    /* Convert index to sequence type */
    seq_t t1 = nn * (nn - 1) / 2;          /* n*(n-1)/2  */
    seq_t t2 = nn * nn + 1;                /* n^2 + 1    */
    return t1 * t2;                        /* Combine to get a(n) */
}
