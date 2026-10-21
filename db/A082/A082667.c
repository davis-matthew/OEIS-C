/*
 * A082667  a(n) = floor(2n/3) * ceiling(2n/3) / 2.
 * https://oeis.org/A082667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1)/3) * (n - floor((n + 1)/3)) (equivalent to
 *            a(n) = floor(2n/3) * ceil(2n/3) / 2)
 * Range:     u8 n <= 34, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m_idx = (n + 1) / 3;           /* floor((n + 1)/3) */
    seq_t m    = (seq_t)m_idx;          /* cast to seq_t */
    seq_t diff = (seq_t)(n - m_idx);     /* n - m_idx */
    return m * diff;                    /* product in seq_t */
}
