/*
 * A041574  Numerators of continued fraction convergents to sqrt(305).
 * https://oeis.org/A041574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of continued‑fraction convergents to sqrt(305)
 *            with a(0)=17, a(1)=35, a(2)=227, ... (periodic CF [17;
 *            overline{2,6,2,34}])
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Standard recurrence for continued fractions:
       p_{-2}=0, p_{-1}=1, p_k = a_k * p_{k-1} + p_{k-2}
       where a_k are the CF coefficients:
           a_0 = 17, a_{k>0} = {2,6,2,34} repeating. */
    seq_t p_prevprev = 0;   /* p_{k-2}   */
    seq_t p_prev     = 1;   /* p_{k-1}   */
    seq_t p_cur;            /* p_k       */
    seq_t a_k;              /* current CF coefficient */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0)
            a_k = 17;
        else
        {
            /* periodic part of length 4: 2,6,2,34 */
            idx_t idx = (k - 1) % 4;
            switch (idx)
            {
            case 0: a_k = 2; break;
            case 1: a_k = 6; break;
            case 2: a_k = 2; break;
            default: a_k = 34; break;
            }
        }

        p_cur = a_k * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_cur;
    }

    return p_prev;  /* p_n */
}
