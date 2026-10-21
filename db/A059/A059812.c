/*
 * A059812  Let g_n be the ball packing n-width for the manifold torus X
 *          interval; sequence gives denominator of (g_n/Pi)^2.
 * https://oeis.org/A059812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n for n >= 9; for n = 1..8 the values are 1, 4, 4, 4, 25,
 *            25, 64, 289 respectively
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)          // undefined for n < 1; return 0 safely
        return (seq_t)0;

    /* pre‑computed values for n = 1..8 */
    static const seq_t tbl[9] = {
        0,                /* dummy for index 0 */
        (seq_t)1,
        (seq_t)4,
        (seq_t)4,
        (seq_t)4,
        (seq_t)25,
        (seq_t)25,
        (seq_t)64,
        (seq_t)289
    };

    if (n < 9)
        return tbl[(size_t)n];

    return (seq_t)n;
}
