/*
 * A211546  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w = 3x-3y.
 * https://oeis.org/A211546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3)*(2*n-1-floor(n/3))/2
 * Range:     u8 n <= 31, u16 n <= 486, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;                     /* m = floor(n/3) */
    seq_t n_seq = (seq_t)n;
    seq_t m_seq = (seq_t)m;
    seq_t res = m_seq * n_seq              /* m*n */
               - (m_seq * (m_seq + 1)) / 2; /* subtract m*(m+1)/2 */
    return res;
}
