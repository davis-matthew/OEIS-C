/*
 * A010145  Continued fraction for sqrt(61).
 * https://oeis.org/A010145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n‑th term of the continued fraction expansion of
 *            sqrt(61)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 94 terms through n = 9941 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The constant square root of 61 (the radical). */
    const idx_t D   = 61;
    const idx_t a0  = 7;  /* floor(sqrt(61))  */

    if (n == 0) return a0;

    idx_t m   = 0;   /* m_0 */
    idx_t d   = 1;   /* d_0 */
    idx_t a_val = a0;/* a_0 */

    /* Iterate the continued fraction recurrence n times to get a(n). */
    for (idx_t i = 1; i <= n; ++i)
    {
        m     = d * a_val - m;
        d     = (D - m * m) / d;
        a_val = (a0 + m) / d;
    }
    return a_val;
}
