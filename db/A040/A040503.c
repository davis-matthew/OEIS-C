/*
 * A040503  Continued fraction for sqrt(526).
 * https://oeis.org/A040503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction terms for sqrt(526)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(526) */
    if (n == 0)
        return (seq_t)22;

    const uint64_t D = 526ULL;          /* the radicand */
    const uint64_t a0 = 22ULL;         /* floor(sqrt(D)) */

    uint64_t m = 0;                    /* m0 */
    uint64_t d = 1;                    /* d0 */
    uint64_t a_val = a0;               /* a0 */

    /* iterate n times to reach a(n) */
    for (idx_t i = 1; i <= n; ++i)
    {
        m = d * a_val - m;
        d = (D - m * m) / d;          /* d fits in 64‑bit (m ≤ sqrt(D)) */
        a_val = (a0 + m) / d;
    }

    return (seq_t)a_val;
}
