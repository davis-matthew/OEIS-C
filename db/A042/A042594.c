/*
 * A042594  Numerators of continued fraction convergents to sqrt(826).
 * https://oeis.org/A042594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            for sqrt(826)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 17, u64 n <= 36, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  p_{-2}=0, p_{-1}=1,  p_k = a_k * p_{k-1} + p_{k-2}  with a_0 = 28 \
 *              and the partial quotients a_k (k>=1) are periodic with period 24:
 *              [1,2,1,5,1,1,1,3,5,2,9,8,9,2,5,3,1,1,1,5,1,2,1,56] */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(826) */
    static const int period[24] = {
        1, 2, 1, 5, 1, 1, 1, 3,
        5, 2, 9, 8, 9, 2, 5, 3,
        1, 1, 1, 5, 1, 2, 1, 56
    };
    const int PERIOD_LEN = 24;

    /* Initial values for the recurrence (p_{-2}=0, p_{-1}=1) */
    seq_t p_m2 = 0;    /* p_{k-2}  */
    seq_t p_m1 = 1;    /* p_{k-1}  */
    seq_t p_k = 0;     /* current p_k */

    /* Iterate from k = 0 to n to compute p_n */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)28;
        } else {
            /* Periodicity of the partial quotients */
            size_t idx = (size_t)((k - 1) % PERIOD_LEN);
            a_k = (seq_t)period[idx];
        }
        p_k = a_k * p_m1 + p_m2;
        /* Shift for next iteration */
        p_m2 = p_m1;
        p_m1 = p_k;
    }

    return p_k;
}
