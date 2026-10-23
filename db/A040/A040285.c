/*
 * A040285  Continued fraction for sqrt(303).
 * https://oeis.org/A040285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction expansion of sqrt(303)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is the integer part of sqrt(303) */
    if (n == 0) return (seq_t)17;

    /* constants for sqrt(303) */
    const uint64_t D   = 303;
    const uint64_t a0  = 17;        /* floor(sqrt(D)) */

    /* intermediate state of the continued‑fraction algorithm */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_val = a0;

    /* generate terms up to a(n) */
    for (idx_t i = 1; i <= n; ++i)
    {
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;     /* integer division = floor */
    }

    return (seq_t)a_val;
}
