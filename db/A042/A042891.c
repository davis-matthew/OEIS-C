/*
 * A042891  Denominators of continued fraction convergents to sqrt(977).
 * https://oeis.org/A042891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Computes the nth denominator of the continued‑fraction
 *            convergents * to sqrt(977). The sequence follows the recurrence
 *            * q_-2 = 1, q_-1 = 0, * q_n = a_n * q_{n-1} + q_{n-2}, * where
 *            the partial quotients are * a_0 = 31 * a_1..a_19 =
 *            [3,1,8,5,1,1,3,7,1,1,7,3,1,1,5,8,1,3,62] * and then repeat
 *            with...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types used by the build */

seq_t a(idx_t n)
{
    static const seq_t period[19] = {
        3ull, 1ull, 8ull, 5ull, 1ull, 1ull, 3ull, 7ull,
        1ull, 1ull, 7ull, 3ull, 1ull, 1ull, 5ull, 8ull,
        1ull, 3ull, 62ull
    };
    const seq_t a0 = 31ull;

    seq_t prev2 = 1;   /* q_-2 */
    seq_t prev1 = 0;   /* q_-1 */
    seq_t cur   = 0;   /* q_n */

    /* The recurrence works for all n >= 0. */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_term;
        if (i == 0)
            a_term = a0;
        else
            a_term = period[(i - 1) % 19];

        cur = a_term * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
