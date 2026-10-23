/*
 * A041800  Numerators of continued fraction convergents to sqrt(421).
 * https://oeis.org/A041800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) are the numerators of the convergents to sqrt(421). The
 *            recurrence is p_(-2) = 0 , p_(-1) = 1 p_k = a_k * p_(k-1) +
 *            p_(k-2) for k ≥ 0 with a_0 = 20 and, for k ≥ 1, a_k cycles with
 *            period 37:
 *            {1,1,13,5,1,3,1,2,1,1,1,2,9,1,7,3,3,2,2,3,3,7,1,9,2,1,1,1,2,1,3,1,5,13,1,1,40}.
 *            The first term a(0) =...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Partial quotients of sqrt(421) for k ≥ 1, period 37. */
    static const seq_t period[37] = {
        1, 1, 13, 5, 1, 3, 1, 2, 1, 1,
        1, 2, 9, 1, 7, 3, 3, 2, 2, 3,
        3, 7, 1, 9, 2, 1, 1, 1, 2, 1,
        3, 1, 5, 13, 1, 1, 40
    };

    if (n == 0)
        return (seq_t)20;              /* a(0) */

    /* p_{-1} = 1 , p_0 = 20 */
    seq_t prev2 = (seq_t)1;           /* p_{-1} */
    seq_t prev1 = (seq_t)20;          /* p_0 */
    seq_t curr;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_i = period[(i - 1) % 37];
        curr = a_i * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;                     /* p_n = a(n) */
}
