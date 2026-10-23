/*
 * A041124  Numerators of continued fraction convergents to sqrt(71).
 * https://oeis.org/A041124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of continued fraction convergents to sqrt(71)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial quotients (after a0 = 8) repeat with period 8: 2,2,1,7,1,2,2,16 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const uint8_t period[8] = {2, 2, 1, 7, 1, 2, 2, 16};
    size_t period_idx = 0;

    seq_t p_prev2 = 0;    /* p_{-2} */
    seq_t p_prev1 = 1;    /* p_{-1} */
    seq_t p = 0;          /* current numerator */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = 8;          /* a0 */
        } else {
            a_k = period[period_idx++];
            if (period_idx == 8) period_idx = 0;
        }
        p = ((seq_t)a_k) * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
