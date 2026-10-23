/*
 * A042224  Numerators of continued fraction convergents to sqrt(638).
 * https://oeis.org/A042224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 25, a(1) = 76, a(n) = c[n-1] * a(n-1) + a(n-2), where the
 *            coefficient sequence c repeats the 8‑term cycle 3,1,6,2,6,1,3,50
 *            (the partial quotients of sqrt(638) after the integer part)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Partial quotients after the integer part of sqrt(638) */
    static const uint8_t period[8] = {3, 1, 6, 2, 6, 1, 3, 50};

    /* p_{-2}=0, p_{-1}=1  (standard continued fraction recurrence) */
    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)25;                 /* a0 */
        else
            coeff = (seq_t)period[(i - 1) & 7]; /* periodic part */

        seq_t p = coeff * p_prev + p_prevprev; /* p_i = coeff * p_{i-1} + p_{i-2} */

        p_prevprev = p_prev;
        p_prev = p;
    }

    return p_prev;
}
