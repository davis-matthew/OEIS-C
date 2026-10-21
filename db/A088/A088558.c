/*
 * A088558  Least odd leg of primitive Pythagorean triangles with even leg 4n.
 * https://oeis.org/A088558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd leg of a primitive Pythagorean triangle with
 *            even leg 4*n. The triangle is parameterised by coprime m > n, of
 *            opposite parity, with even leg 2*m*n = 4*n. For each factor pair
 *            m*n = 2*n we compute m^2 - n^2 and take the minimum
 * Range:     u8 n <= 8, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe */
    uint64_t N = (uint64_t)n;
    uint64_t P = 2 * N;            /* product m*n = 2*n */
    seq_t best = 0;
    int first = 1;
    for (uint64_t d = 1; d * d <= P; ++d) {
        if (P % d != 0) continue;
        uint64_t n_val = d;
        uint64_t m_val = P / d;   /* m > n by construction if m_val > n_val */
        if (m_val <= n_val) continue;
        if (gcd_uint64(m_val, n_val) != 1) continue;
        if ((m_val & 1ULL) == (n_val & 1ULL)) continue;   /* opposite parity */
        seq_t odd_leg = (seq_t)(m_val * m_val - n_val * n_val);
        if (first || odd_leg < best) {
            best = odd_leg;
            first = 0;
        }
    }
    return best;
}
