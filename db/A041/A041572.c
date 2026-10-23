/*
 * A041572  Numerators of continued fraction convergents to sqrt(304).
 * https://oeis.org/A041572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent in the continued
 *            fraction expansion of sqrt(304). Recurrence (with a0 = 17,
 *            period = [2,3,2,1,1,1,1,1,2,3,2,34]): p_{-2} = 0, p_{-1} = 1 p_n
 *            = a_n * p_{n-1} + p_{n-2} for n ≥ 0 a_n = 17 if n == 0 a_n =
 *            period[(n-1) % 12] otherwise
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* period of partial quotients for sqrt(304) after the first term */
    static const uint16_t period[12] = {
        2, 3, 2, 1, 1, 1, 1, 1, 2, 3, 2, 34
    };

    seq_t p_nm2 = 0;          /* corresponds to p_{-2} */
    seq_t p_nm1 = 1;          /* corresponds to p_{-1} */
    seq_t p_n;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == 0)
            a_i = 17;
        else
            a_i = (seq_t)period[(i - 1) % 12];

        /* p_n = a_i * p_{n-1} + p_{n-2} */
        p_n = a_i * p_nm1 + p_nm2;

        /* shift for next iteration */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_n;
}
