/*
 * A040467  Continued fraction for sqrt(490).
 * https://oeis.org/A040467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = the n-th partial quotient of the continued fraction of
 *            sqrt(490)
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    const seq_t D = 490;
    seq_t a0 = floor_sqrt((seq_t)D);     /* floor(sqrt(490)) = 22 */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = a0;

    if (n == 0u)
        return a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;
    }

    return a_val;
}
