/*
 * A042178  Numerators of continued fraction convergents to sqrt(614).
 * https://oeis.org/A042178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = a_{n-1} * a_n + a_{n-2} with a_0 = 24, and the partial
 *            quotients a_k for the continued fraction of sqrt(614) repeat
 *            with period
 *            [1,3,1,1,9,2,1,4,3,1,1,2,24,2,1,1,3,4,1,2,9,1,1,3,1,48]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the n-th convergent to sqrt(614). */
#include <stdint.h>

static const idx_t period[26] = {
    1U, 3U, 1U, 1U, 9U, 2U, 1U, 4U, 3U, 1U,
    1U, 2U, 24U, 2U, 1U, 1U, 3U, 4U, 1U, 2U,
    9U, 1U, 1U, 3U, 1U, 48U
};

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p, c;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            c = (seq_t)24;  /* a_0 */
        } else {
            /* a_k for k>=1: period[(k-1) % 26] */
            c = period[(k - 1) % 26];
        }
        p = c * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
