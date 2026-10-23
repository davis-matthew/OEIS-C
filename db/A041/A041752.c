/*
 * A041752  Numerators of continued fraction convergents to sqrt(396).
 * https://oeis.org/A041752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent of the continued *
 *            fraction of sqrt(396). The continued fraction expansion is *
 *            [19;(1,8,1,38)] repeating. Therefore the recurrence is * * a(0)
 *            = 19 * p_{-2} = 0, p_{-1} = 1 * For i ≥ 0: * let a_i = 19 if
 *            i==0 * = period[(i-1) mod 4] otherwise (period...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t PERIOD[4] = { (seq_t)1, (seq_t)8, (seq_t)1, (seq_t)38 };

seq_t a(idx_t n)
{
    /* Handle the base case directly in the loop. */
    seq_t p_prevprev = (seq_t)0; /* p_{i-2} */
    seq_t p_prev     = (seq_t)1; /* p_{i-1} */
    seq_t curr       = (seq_t)0; /* p_i */
    idx_t i;

    for (i = 0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)19;
        else
            a_i = PERIOD[(i - 1) % 4];

        /* Compute p_i = a_i * p_{i-1} + p_{i-2} */
        curr = a_i * p_prev + p_prevprev;

        if (i == n)
            return curr;

        /* Shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = curr;
    }

    /* Unreachable: n >= 0 guarantees return above. */
    return (seq_t)0;
}
