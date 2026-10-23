/*
 * A041594  Numerators of continued fraction convergents to sqrt(315).
 * https://oeis.org/A041594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 17; for n ≥ 1, a(n) satisfies a(n) = k*a(n‑1) + a(n‑2)
 *            where k cycles through {1, 2, 1, 34} (i.e. k = period[(n‑1) %
 *            4]). This recurrence generates the numerators of the
 *            continued‑fraction convergents to √315
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic multipliers for the recurrence after a0 */
    static const int period[4] = {1, 2, 1, 34};

    seq_t prev2 = 0;  /* p_{-2} */
    seq_t prev1 = 1;  /* p_{-1} */
    seq_t curr  = 0;  /* current numerator */

    /* Iterate from 0 up to n */
    for (idx_t i = 0; i <= n; ++i) {
        int k;              /* multiplier */
        if (i == 0)
            k = 17;        /* a0 */
        else
            k = period[(i - 1) % 4];

        if (i == 0) {
            curr = (seq_t)k;
        } else {
            curr = (seq_t)k * prev1 + prev2;
        }

        /* shift */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
