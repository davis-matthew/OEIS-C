/*
 * A042658  Numerators of continued fraction convergents to sqrt(859).
 * https://oeis.org/A042658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th continued fraction convergent to
 *            sqrt(859)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 18, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Partial quotients of sqrt(859) after the initial term a0 = 29.
       The period length is 46: a1..a46. */
    static const seq_t cf[46] = {
        3, 4, 5, 1, 1, 1, 2, 2, 3, 2,
        19, 9, 1, 2, 1, 1, 4, 1, 3, 11,
        2, 6, 29, 6, 2, 11, 3, 1, 4, 1,
        1, 2, 1, 9, 19, 2, 3, 2, 2, 1,
        1, 1, 5, 4, 3, 58
    };
    const idx_t PERIOD = 46;

    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p_n   = 0;   /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = 29;          /* a0 */
        } else {
            a_i = cf[(i - 1) % PERIOD];
        }
        p_n = a_i * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_n;
}
