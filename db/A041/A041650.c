/*
 * A041650  Numerators of continued fraction convergents to sqrt(344).
 * https://oeis.org/A041650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(344) continued
 *            fraction
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the partial quotients of sqrt(344) are: 18; [1,1,4,1,3,1,4,1,1,36] repeating */

#include <stddef.h>

/* NOTE: idx_t and seq_t are provided by the build system. */

static const idx_t a0      = (idx_t)18;
static const idx_t period_len = 10u;
static const idx_t period_coeff[10] = {
    1, 1, 4, 1, 3, 1, 4, 1, 1, 36
};

seq_t a(idx_t n)
{
    /* recurrence for numerators: p[-2]=0, p[-1]=1 */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t p_curr;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t coeff = (i == 0) ? a0 : period_coeff[(i - 1) % period_len];
        p_curr = (seq_t)coeff * p_prev1 + p_prev2;   /* p_i = coeff * p_{i-1} + p_{i-2} */

        if (i == n)
            return p_curr;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
