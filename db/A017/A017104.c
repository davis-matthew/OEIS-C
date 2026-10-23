/*
 * A017104  a(n) = (8*n+3)^4.
 * https://oeis.org/A017104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 31, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute m = 8*n + 3  */
    seq_t m = (seq_t)n * 8 + 3;

    /* compute m^2 */
    seq_t m_sq = seq_mul(m, m);

    /* compute (m^2)^2 = m^4 */
    return seq_mul(m_sq, m_sq);
}
