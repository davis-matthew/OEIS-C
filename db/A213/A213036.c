/*
 * A213036  a(n) = n^2 - floor(2n/3)^2.
 * https://oeis.org/A213036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - floor(2n/3)^2
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* n^2 */
    seq_t n_sq = n_seq * n_seq;

    /* floor(2n/3) computed with 128‑bit intermediate to avoid overflow */
    idx_t m = (unsigned __int128)n * 2 / 3;
    seq_t m_seq = (seq_t)m;

    /* (floor(2n/3))^2 */
    seq_t m_sq = m_seq * m_seq;

    /* a(n) = n^2 - floor(2n/3)^2 */
    return n_sq - m_sq;
}
