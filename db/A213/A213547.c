/*
 * A213547  Antidiagonal sums of the convolution array A213505.
 * https://oeis.org/A213547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 6*n^5 + 15*n^4 + 20*n^3 + 14*n^2 + 4*n)/60
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 78, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* promote the index to seq_t, then to unsigned __int128 for safe multiplication */
    const seq_t sn = (seq_t)n;
    unsigned __int128 x = (unsigned __int128)sn;   /* x = n */

    unsigned __int128 x2  = x * x;    /* n^2 */
    unsigned __int128 x3  = x2 * x;   /* n^3 */
    unsigned __int128 x4  = x3 * x;   /* n^4 */
    unsigned __int128 x5  = x4 * x;   /* n^5 */
    unsigned __int128 x6  = x5 * x;   /* n^6 */

    /* compute the polynomial in 128‑bit arithmetic */
    unsigned __int128 poly = x6
                           + 6 * x5
                           + 15 * x4
                           + 20 * x3
                           + 14 * x2
                           + 4 * x;

    /* integer division by 60 */
    return (seq_t)(poly / 60);
}
