/*
 * A042082  Numerators of continued fraction convergents to sqrt(565).
 * https://oeis.org/A042082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = a_{n-1}*q_n + a_{n-2}, where a_{-2}=0, a_{-1}=1, q_0=23
 *            and * for k≥1 the sequence q_k is the periodic partial quotients
 *            of √565: * [1,3,2,1,11,5,5,11,1,2,3,1,46] (period length 13)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t period[13] = {1,3,2,1,11,5,5,11,1,2,3,1,46};
    seq_t p_prev2 = 0;   /* a_{-2} */
    seq_t p_prev1 = 1;   /* a_{-1} */
    seq_t p = 0;         /* a_n */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff;
        if (k == 0) {
            coeff = 23;                /* q_0 */
        } else {
            coeff = period[(size_t)((k - 1) % 13)];
        }
        p = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
