/*
 * A041996  Numerators of continued fraction convergents to sqrt(521).
 * https://oeis.org/A041996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = the numerator of the n-th convergent of the
 *            continued‑fraction expansion of √521
 * Range:     u8 none, u16 n <= 9, u32 n <= 18, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const idx_t D = 521;                     /* square‑free D */
    seq_t a0 = floor_sqrt((seq_t)D);         /* a0 = ⌊√D⌋ = 22 */

    if (n == 0)
        return a0;                           /* h0 = a0 */

    /* integer part of the continued fraction (fits in idx_t) */
    idx_t a0_i = (idx_t)a0;                  /* 22 */
    idx_t M = 0;                             /* M0 */
    idx_t Dp = 1;                            /* D'0 */
    idx_t a_cf = a0_i;                       /* a0 */

    /* convergent numerators: h_{-2} = 0, h_{-1} = 1, h0 = a0 */
    seq_t h_prevprev = 1;                    /* h_{-1} */
    seq_t h_prev = a0;                       /* h0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* next elements of the continued fraction */
        idx_t Mnext = a_cf * Dp - M;
        idx_t Dnext = (D - Mnext * Mnext) / Dp;
        idx_t a_next = (a0_i + Mnext) / Dnext;

        /* numerator recurrence: h_k = a_k*h_{k-1} + h_{k-2} */
        seq_t h_next = (seq_t)a_next * h_prev + h_prevprev;

        /* shift for next iteration */
        h_prevprev = h_prev;
        h_prev = h_next;
        M = Mnext;
        Dp = Dnext;
        a_cf = a_next;
    }

    return h_prev;                          /* h_n */
}
