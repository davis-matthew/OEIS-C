/*
 * A184241  Number of strings of numbers x(i=1..3) in 0..n with sum i^2*x(i)^2
 *          equal to n^2*9.
 * https://oeis.org/A184241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(x1,x2,x3) | 0 ≤ xi ≤ n , x1² + 4*x2² + 9*x3² = 9*n² }|
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t N = n;
    seq_t nn = (seq_t)N;
    seq_t n2 = nn * nn;
    seq_t count = 0;

    for (idx_t x3 = 0; x3 <= N; ++x3) {
        seq_t x3s = (seq_t)x3 * x3;
        seq_t T = n2 - x3s;               /* T ≥ 0 */
        seq_t right = (seq_t)9 * T;        /* x1² + 4*x2² = right */

        /* Upper bound for x2 */
        seq_t div4 = right / 4;
        seq_t sqrtDiv4 = floor_sqrt(div4);
        idx_t max_x2 = (sqrtDiv4 > nn) ? N : (idx_t)sqrtDiv4;

        for (idx_t x2 = 0; x2 <= max_x2; ++x2) {
            seq_t x2s = (seq_t)x2 * x2;
            seq_t R = right - 4 * x2s;
            if (R < 0) continue;           /* sanity check */

            seq_t x1 = floor_sqrt(R);
            if (x1 * x1 == R && x1 <= nn) {
                ++count;
            }
        }
    }

    return count;
}
