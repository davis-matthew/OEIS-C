/*
 * A042315  Denominators of continued fraction convergents to sqrt(684).
 * https://oeis.org/A042315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(684)
 * Range:     u8 none, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D  = (seq_t)684;
    const seq_t a0 = floor_sqrt(D);

    seq_t m = 0;
    seq_t d = 1;
    seq_t q_pp = 1; /* q_{-2} */
    seq_t q_p  = 0; /* q_{-1} */
    seq_t q_c;

    /* k = 0 */
    q_c = a0 * q_p + q_pp;  /* = 1 */
    if (n == 0)
        return q_c;

    q_pp = q_p;
    q_p  = q_c;

    seq_t a = a0;
    for (idx_t k = 1; k <= n; ++k) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        q_c = a * q_p + q_pp;
        if (k == n)
            return q_c;
        q_pp = q_p;
        q_p  = q_c;
    }

    return 0; /* never reached */
}
