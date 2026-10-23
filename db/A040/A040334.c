/*
 * A040334  Continued fraction for sqrt(353).
 * https://oeis.org/A040334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(353) */
/* Sequence A040334 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D = 353ULL;
    /* compute a0 = floor(sqrt(D)) */
    uint64_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) a0++;

    if (n == 0) return (seq_t)a0;

    uint64_t m = 0, d = 1, a = a0;
    for (idx_t i = 1; i <= n; ++i) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }
    return (seq_t)a;
}
