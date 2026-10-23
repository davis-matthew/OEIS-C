/*
 * A041230  Numerators of continued fraction convergents to sqrt(127).
 * https://oeis.org/A041230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(127).
   Let a_0 = 11 and for k ≥ 1 let
        a_k = { 3,1,2,2,7,11,7,2,2,1,3,22 } repeating.
   Define p_{-2} = 0,  p_{-1} = 1.
   Then p_k = a_k * p_{k-1} + p_{k-2}  (k ≥ 0).
   The sequence a(n) is the k-th numerator p_k (a(0) = 11). */

#include <stddef.h>

#define PERIOD_LEN 12
static const idx_t period[PERIOD_LEN] = {
    3, 1, 2, 2, 7, 11, 7, 2, 2, 1, 3, 22
};

seq_t a(idx_t n)
{
    /* Working variables: p_{k-2} and p_{k-1} */
    seq_t p_prevprev = (seq_t)0;    /* p_{-2} */
    seq_t p_prev = (seq_t)1;        /* p_{-1} */
    seq_t p = (seq_t)0;            /* current p_k */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)11; /* a_0 */
            p = coeff * p_prev + p_prevprev; /* 11*1 + 0 */
        } else {
            coeff = (seq_t)period[(i - 1) % PERIOD_LEN];
            if (i == 1) {
                p = coeff * p_prev + (seq_t)1; /* p_{-1} = 1 explicitly */
            } else {
                p = coeff * p_prev + p_prevprev;
            }
        }
        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev = p;
    }
    return p;
}
