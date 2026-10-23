/*
 * A015220  Even tetrahedral numbers.
 * https://oeis.org/A015220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T_{m} where m = 4*floor(n/3) + {0,2,3} depending on n%3,
 *            and T_{m} = m*(m+1)*(m+2)/6 (tetrahedral number)
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine m, the index of the tetrahedral number to use */
    idx_t block  = n / 3;
    idx_t offset = n % 3;
    idx_t m = block * 4 + (offset == 0 ? 0 : (offset == 1 ? 2 : 3));

    /* Compute tetrahedral number T_m = m*(m+1)*(m+2)/6 safely */
    seq_t m_seq = (seq_t)m;
    seq_t t = m_seq * (m_seq + 1);   /* m*(m+1)      */
    t /= 2;                          /* divide by 2   */
    t *= (m_seq + 2);                /* * (m+2)      */
    t /= 3;                          /* divide by 3   */
    return t;
}
