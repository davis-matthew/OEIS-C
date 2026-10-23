/*
 * A168372  a(n) = n^5*(n^4 + 1)/2.
 * https://oeis.org/A168372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 * (n^4 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 sn = n;                  /* n  */
    unsigned __int128 n2 = sn * sn;            /* n^2 */
    unsigned __int128 n4 = n2 * n2;            /* n^4 */
    unsigned __int128 n5 = n4 * sn;            /* n^5 */
    unsigned __int128 prod = n5 * (n4 + 1);    /* n^5 * (n^4 + 1) */
    prod /= 2;                                 /* divide by 2 */
    return (seq_t)prod;                        /* cast back to seq_t */
}
