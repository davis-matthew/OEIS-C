/*
 * A042304  Numerators of continued fraction convergents to sqrt(679).
 * https://oeis.org/A042304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of √679. Recurrence: let
 *            a0 = ⌊√679⌋ = 26. Set m0 = 0, d0 = 1, a0 = ⌊√679⌋. For k ≥ 0
 *            define P_{-2} = 0, P_{-1} = 1, P_k = a_k · P_{k-1} + P_{k-2},
 *            where the continued‑fraction terms a_k are obtained by the
 *            standard recurrence: m_{k+1} = d_k · a_k – m_k d_{...
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const uint64_t D = 679ULL;
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a0 = (uint64_t)floor_sqrt((seq_t)D);   /* a0 = ⌊√679⌋ = 26 */
    uint64_t a = a0;                                 /* current CF term */

    seq_t p_prev2 = (seq_t)0;   /* P_{-2} */
    seq_t p_prev1 = (seq_t)1;   /* P_{-1} */
    seq_t p = (seq_t)0;         /* P_k */

    for (idx_t k = 0; k <= n; ++k) {
        if (k != 0) {           /* generate next continued‑fraction term */
            uint64_t m1 = d * a - m;
            uint64_t d1 = (D - m1 * m1) / d;
            a = (a0 + m1) / d1;
            m = m1;
            d = d1;
        }
        p = (seq_t)a * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
