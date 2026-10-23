/*
 * A042586  Numerators of continued fraction convergents to sqrt(822).
 * https://oeis.org/A042586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerators of continued fraction convergents to sqrt(822)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    /* Partial quotients after a0 = 28: 1, 2, 28, 2, 1, 56 */
    static const unsigned short period[6] = {1, 2, 28, 2, 1, 56};
    seq_t p_prev2 = 0;           /* p(-2) */
    seq_t p_prev1 = 1;           /* p(-1) */
    seq_t p_curr = 0;
    idx_t i;
    for (i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)28;
        else
            coeff = (seq_t)period[(size_t)((i - 1) % 6)];
        p_curr = coeff * p_prev1 + p_prev2;
        if (i == n)
            break;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_curr;
}
