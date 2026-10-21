/*
 * A064876  Last of four sequences representing the lexicographical minimal
 *          decomposition of n in 4 squares: n = A064873(n)^2 + A064874(n)^2 +
 *          A064875(n)^2 + a(n)^2.
 * https://oeis.org/A064876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the last component of the lexicographically minimal quadruple
   (x1,x2,x3,x4) of non‑negative integers with
   n = x1^2 + x2^2 + x3^2 + x4^2 and x1 ≤ x2 ≤ x3 ≤ x4.  This is the
   definition of OEIS A064876. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    for (seq_t x1 = 0; x1 * x1 <= N; ++x1) {
        seq_t rem1 = N - x1 * x1;
        for (seq_t x2 = x1; x2 * x2 <= rem1; ++x2) {
            seq_t rem2 = rem1 - x2 * x2;
            for (seq_t x3 = x2; x3 * x3 <= rem2; ++x3) {
                seq_t rem3 = rem2 - x3 * x3;
                seq_t x4 = floor_sqrt(rem3);
                if (x4 * x4 == rem3 && x4 >= x3)
                    return x4;
            }
        }
    }
    return 0;  /* should never be reached */
}
