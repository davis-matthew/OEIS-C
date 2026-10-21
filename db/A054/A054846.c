/*
 * A054846  Nearest integer to n^(2/3).
 * https://oeis.org/A054846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nearest integer to n^(2/3)
 * Range:     u8 n <= 7, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* n² as seq_t */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* floor of n^(2/3) : floor_cube_root(n²) */
    seq_t m = floor_cube_root(sq);

    /* 8 * n²  (left side of the inequality 8n² < (2m+1)³) */
    seq_t left = (seq_t)8 * sq;

    /* (2m+1)³  (right side of the inequality) */
    seq_t two_m_plus1 = (m << 1) + 1;
    seq_t right = two_m_plus1 * two_m_plus1 * two_m_plus1;

    /* choose floor or ceil according to the comparison */
    return (left < right) ? m : m + 1;
}
