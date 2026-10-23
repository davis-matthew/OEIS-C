/*
 * A040588  Continued fraction for sqrt(613).
 * https://oeis.org/A040588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction terms for sqrt(613)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t root = 613ULL;
    const uint64_t a0_val = 24ULL;      /* floor(sqrt(613)) */

    if (n == 0) return (seq_t)a0_val;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_val = a0_val;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a_val - m;
        d = (root - m * m) / d;
        a_val = (a0_val + m) / d;
    }

    return (seq_t)a_val;
}
