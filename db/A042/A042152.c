/*
 * A042152  Numerators of continued fraction convergents to sqrt(601).
 * https://oeis.org/A042152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 19, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042152: numerators of continued‑fraction convergents to sqrt(601)
   a(0) = 24
   a(n) = a(n‑1) * p_n + a(n‑2),   n ≥ 1
   where the partial quotients p_n (n ≥ 1) repeat with period
   1, 1, 15, 1, 5, 5, 3, 1, 1, 2, 1, 2, 2, 1, 9, 9, 1, 2, 2, 1,
   2, 1, 1, 3, 5, 5, 1, 15, 1, 1, 48
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* partial quotients after the initial term a0 = 24 */
    static const unsigned period[] =
    {
        1, 1, 15, 1, 5, 5, 3, 1, 1, 2, 1, 2, 2, 1, 9, 9, 1, 2, 2, 1,
        2, 1, 1, 3, 5, 5, 1, 15, 1, 1, 48
    };
    const idx_t L = sizeof(period) / sizeof(period[0]);

    seq_t prev2 = 0;          /* p_{-2} */
    seq_t prev1 = 1;          /* p_{-1} */
    seq_t cur   = 0;          /* p_i */

    for (idx_t i = 0; i <= n; ++i)
    {
        unsigned a_i = (i == 0) ? 24
                                : period[(i - 1) % L];
        cur = (seq_t)a_i * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
