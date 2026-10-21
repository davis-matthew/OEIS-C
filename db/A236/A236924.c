/*
 * A236924  Number of integer solutions to a^2 + 2*b^2 + 2*c^2 + 4*d^2 = n.
 * https://oeis.org/A236924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + 2*b^2 + 2*c^2 +
 *            4*d^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over non‑negative a */
    idx_t max_a = (idx_t)floor_sqrt((seq_t)n);
    for (idx_t a = 0; a <= max_a; ++a) {
        seq_t a2       = (seq_t)a * a;
        seq_t mult_a   = (a == 0 ? 1 : 2);   /* sign for a */
        seq_t rem_ab   = (seq_t)n - a2;
        seq_t div2_ab  = rem_ab / 2;
        idx_t max_b    = (idx_t)floor_sqrt(div2_ab);

        for (idx_t b = 0; b <= max_b; ++b) {
            seq_t b2       = (seq_t)b * b;
            seq_t mult_b   = (b == 0 ? 1 : 2);   /* sign for b */
            seq_t s_b      = a2 + 2 * b2;
            seq_t rem_bc   = (seq_t)n - s_b;
            seq_t div2_bc  = rem_bc / 2;
            idx_t max_c    = (idx_t)floor_sqrt(div2_bc);

            for (idx_t c = 0; c <= max_c; ++c) {
                seq_t c2     = (seq_t)c * c;
                seq_t mult_c = (c == 0 ? 1 : 2);   /* sign for c */
                seq_t s      = s_b + 2 * c2;
                seq_t rem    = (seq_t)n - s;

                if ((rem & 3) != 0)       /* rem % 4 != 0 */
                    continue;

                seq_t d2     = rem >> 2;   /* rem / 4 */
                seq_t d      = floor_sqrt(d2);
                if (d * d != d2)           /* not a perfect square */
                    continue;

                seq_t mult_d = (d == 0 ? 1 : 2);   /* sign for d */
                result += mult_a * mult_b * mult_c * mult_d;
            }
        }
    }
    return result;
}
