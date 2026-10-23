/*
 * A041632  Numerators of continued fraction convergents to sqrt(335).
 * https://oeis.org/A041632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=18, a(1)=55, and for n>=2 * a(n) = (n%4==0 ? 36 : 3)*a(n-1)
 *            + a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0;      /* p_{-2} */
    seq_t p_prev1 = 1;      /* p_{-1} */
    seq_t coeff;
    seq_t p_cur;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            coeff = 18;
        } else {
            coeff = (k % 4 == 0) ? 36 : 3;
        }
        p_cur = coeff * p_prev1 + p_prev2;
        if (k == n) {
            return p_cur;
        }
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }
    return 0; /* unreachable */
}
