/*
 * A041520  Numerators of continued fraction convergents to sqrt(277).
 * https://oeis.org/A041520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) are the numerators of the continued fraction convergents *
 *            to sqrt(277). The continued fraction of sqrt(277) is * [16;
 *            1,1,1,4,10,1,7,2,2,3,3,2,2,7,1,10,4,1,1,1,32] repeating, * i.e.
 *            the coefficient sequence (including the initial term) is * a0 =
 *            16, a1 = 1, a2 = 1, a3 = 1, a4 = 4, a5 = 10,...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)16;
    }

    /* Periodic part of continued fraction (after the initial 16) */
    static const unsigned cf[21] = {
        1, 1, 1, 4, 10, 1, 7, 2, 2, 3,
        3, 2, 2, 7, 1, 10, 4, 1, 1, 1, 32
    };
    const size_t period = 21;

    seq_t p_prevprev = 0;   /* P_{k-2} */
    seq_t p_prev     = 1;   /* P_{k-1} */
    seq_t p_curr     = 0;   /* P_k, will be overwritten */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)16;
        } else {
            coeff = (seq_t)cf[(size_t)((i - 1) % period)];
        }
        p_curr = coeff * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    return p_curr;   /* P_n */
}
