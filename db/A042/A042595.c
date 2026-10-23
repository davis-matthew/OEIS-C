/*
 * A042595  Denominators of continued fraction convergents to sqrt(826).
 * https://oeis.org/A042595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = c[(n-1)%24] * a(n-1) + a(n-2) where c
 *            = {1,2,1,5,1,1,1,3,5,2,9,8,9,2,5,3,1,1,1,5,1,2,1,56}
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Period of the continued fraction of sqrt(826) */
    static const unsigned int period[24] = {
        1, 2, 1, 5, 1, 1, 1, 3, 5, 2,
        9, 8, 9, 2, 5, 3, 1, 1, 1, 5,
        1, 2, 1, 56
    };
    const size_t L = 24;

    seq_t q_prevprev = 1;              /* a(0)   */
    seq_t q_prev     = period[0];      /* a(1) = period[0] */

    if (n == 1)
        return q_prev;

    for (idx_t i = 2; i <= n; ++i) {
        unsigned int coeff = period[(i - 1) % L];
        seq_t prod = q_prev * coeff + q_prevprev;
        q_prevprev = q_prev;
        q_prev = prod;
    }
    return q_prev;
}
