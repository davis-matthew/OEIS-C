/*
 * A054847  Nearest integer to (nearest integer to n^(2/3))^(3/2).
 * https://oeis.org/A054847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nearest integer to (nearest integer to n^(2/3))^(3/2)
 * Range:     u8 n <= 6, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Step 1: m = nearest integer to n^(2/3) */
    seq_t n2 = (seq_t)n * (seq_t)n;                     /* n^2          */
    seq_t m_floor = floor_cube_root(n2);                /* floor(n^(2/3)) */

    /* Compare 8*n^2 with (2*m_floor+1)^3 to decide rounding */
    seq_t lhs1 = (seq_t)8 * n2;
    seq_t two_m = (seq_t)2 * m_floor + (seq_t)1;
    seq_t rhs1 = two_m * two_m * two_m;
    seq_t m = (lhs1 <= rhs1) ? m_floor : (m_floor + (seq_t)1);

    /* Step 2: r = nearest integer to sqrt(m^3) */
    seq_t m3 = m * m * m;
    seq_t s_floor = floor_sqrt(m3);                     /* floor(m^(3/2)) */

    /* Compare 4*m^3 with (2*s_floor+1)^2 to decide rounding */
    seq_t lhs2 = (seq_t)4 * m3;
    seq_t two_s = (seq_t)2 * s_floor + (seq_t)1;
    seq_t rhs2 = two_s * two_s;
    seq_t res = (lhs2 <= rhs2) ? s_floor : (s_floor + (seq_t)1);

    return res;
}
