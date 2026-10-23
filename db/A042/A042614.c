/*
 * A042614  Numerators of continued fraction convergents to sqrt(836).
 * https://oeis.org/A042614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            for sqrt(836), with a(0) = floor(sqrt(836)) = 28. The recurrence
 *            for convergents is h[-2] = 0, h[-1] = 1, h[i] = a_i * h[i-1] +
 *            h[i-2], where a_0 = 28 and a_i (i≥1) are the partial quotients
 *            of sqrt(836). The partial quotients are g...
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* constants for sqrt(836) */
    const idx_t D   = 836ULL;
    const idx_t a0  = 28ULL;   /* floor(sqrt(836)) */

    /* convergent numerators */
    seq_t h_prev2 = 0;   /* h[-2] */
    seq_t h_prev1 = 1;   /* h[-1] */
    seq_t h = 0;         /* current convergent numerator */

    /* continued‑fraction state (starting values) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_val = a0;   /* a_0 */

    /* i = 0 : first convergent (just the integer part) */
    h = (seq_t)a_val * h_prev1 + h_prev2;   /* h0 = a0*1 + 0 = 28 */
    if (n == 0)
        return h;

    /* shift for next iteration */
    h_prev2 = h_prev1;
    h_prev1 = h;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* generate next partial quotient a_i (i≥1) */
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;

        /* compute next convergent numerator */
        h = (seq_t)a_val * h_prev1 + h_prev2;

        /* shift for next iteration */
        h_prev2 = h_prev1;
        h_prev1 = h;
    }

    return h;
}
