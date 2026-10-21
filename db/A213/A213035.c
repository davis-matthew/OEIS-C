/*
 * A213035  a(n) = n^2 - floor(n/3)^2.
 * https://oeis.org/A213035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - floor(n/3)^2
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = floor(n/3) */
    idx_t m_idx = n / 3;
    seq_t m = (seq_t)m_idx;

    /* use 128‑bit arithmetic to avoid overflow before casting back to seq_t */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 m128 = (unsigned __int128)m;

    unsigned __int128 res = n128 * n128 - m128 * m128;

    return (seq_t)res;
}
