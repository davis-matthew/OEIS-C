/*
 * A042774  Numerators of continued fraction convergents to sqrt(918).
 * https://oeis.org/A042774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042774 – numerators of continued fraction convergents to √918.
   Recurrence:  p_{-2}=0,  p_{-1}=1,  p_k = a_k·p_{k-1} + p_{k-2},
   where a_0 = 30 and, for k≥1, a_k repeats the period
   [3, 2, 1, 6, 30, 6, 1, 2, 3, 60]. */
#include <stdint.h>

#define PERIOD 10
static const idx_t PERIOD_COEFFS[PERIOD] = {3, 2, 1, 6, 30, 6, 1, 2, 3, 60};

seq_t a(idx_t n)
{
    seq_t prev2 = 0;     /* p_{-2} */
    seq_t prev1 = 1;     /* p_{-1} */
    seq_t cur    = 0;    /* p_k */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = (seq_t)30;               /* a_0 = floor(√918) */
        } else {
            ai = (seq_t)PERIOD_COEFFS[(i - 1) % PERIOD];
        }
        cur = ai * prev1 + prev2;          /* p_k = a_k·p_{k-1} + p_{k-2} */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;                            /* numerator a(n) */
}
