/*
 * A053436  a(n) = n+1 + ceiling(n/2)*(ceiling(n/2)-1)*(ceiling(n/2)+1)/6.
 * https://oeis.org/A053436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 1 + ceil(n/2) * (ceil(n/2)-1) * (ceil(n/2)+1) / 6
 * Range:     u8 n <= 12, u16 n <= 145, u32 n <= 3543, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = ceil(n/2) */
    idx_t m_idx = (n + 1) / 2;          /* ok for all unsigned n ≥ 1 */
    seq_t m   = (seq_t)m_idx;

    /* Compute the triangular-like term: m*(m-1)*(m+1)/6 */
    seq_t tmp  = m * (m - 1) * (m + 1);
    tmp /= 6;

    /* Final result */
    return ((seq_t)n) + 1 + tmp;
}
