/*
 * A041338  Numerators of continued fraction convergents to sqrt(183).
 * https://oeis.org/A041338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   get_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_a.h"
/* 
 * Sequence: A041338 – Numerators of continued fraction convergents to sqrt(183).
 * Computation method:
 *   Let a_k be the continued fraction partial quotients of sqrt(183):
 *       a_0 = 13,
 *       (a_1 … a_6) = 1, 1, 8, 1, 1, 26,     // period of length 6
 *   The convergent numerators p_k satisfy
 *       p_{-2} = 0,   p_{-1} = 1,
 *       p_k   = a_k * p_{k-1} + p_{k-2}   for k ≥ 0.
 *   The function below returns p_n = a(n).
 */

seq_t a(idx_t n)
{
    /* convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_prevprev = 0;   /* p_{k-2} */
    seq_t p_prev     = 1;   /* p_{k-1} */
    seq_t p_cur      = 0;   /* p_k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff = get_a(k);
        p_cur = coeff * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_cur;
    }
    return p_cur;
}
