/*
 * A041080  Numerators of continued fraction convergents to sqrt(47).
 * https://oeis.org/A041080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(47).
 * Recurrence:
 *   p_{-2} = 0, p_{-1} = 1,
 *   p_k     = a_k * p_{k-1} + p_{k-2},
 * where
 *   a_0 = 6,
 *   a_{k>=1} follows the period {1,5,1,12}.
 */
#include <stdint.h>

static const idx_t period[4] = {1, 5, 1, 12};

seq_t a(idx_t n)
{
    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */
    seq_t newp = 0;       /* current p_k */

    for (idx_t i = 0; i <= n; ++i) {
        /* coefficient a_i */
        seq_t coeff = (i == 0) ? (seq_t)6
                               : (seq_t)period[(i - 1) % 4];
        /* recurrence: p_k = a_k * p_{k-1} + p_{k-2} */
        newp = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = newp;
    }
    return newp;
}
