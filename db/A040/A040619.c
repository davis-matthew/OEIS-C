/*
 * A040619  Continued fraction for sqrt(645).
 * https://oeis.org/A040619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040619: continued fraction terms of √645.
   Computed by the standard algorithm for quadratic surds. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* D is the radicand */
    const seq_t D = (seq_t)645;
    /* Integer part of the square root (a₀) */
    const seq_t a0 = floor_sqrt(D);

    if (n == 0)
        return a0;   /* a(0) = ⌊√645⌋ */

    /* Variables for the continued‑fraction algorithm */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    /* Iterate to the desired index */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t m2 = d * a - m;
        seq_t d2 = (D - m2 * m2) / d;
        seq_t a2 = (a0 + m2) / d2;

        m = m2;
        d = d2;
        a = a2;
    }
    return a;
}
