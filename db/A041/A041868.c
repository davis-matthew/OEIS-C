/*
 * A041868  Numerators of continued fraction convergents to sqrt(456).
 * https://oeis.org/A041868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=21; for n>0: a(n)=coeff[n-1]*a(n-1)+a(n-2). 
   coeff cycles through {2,1,4,1,2,42} (period 6), i.e. 
   a(1)=2*a(0)+a(-1) where a(-1)=0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t coeff[6] = {(seq_t)2, (seq_t)1, (seq_t)4,
                                   (seq_t)1, (seq_t)2, (seq_t)42};
    seq_t p_prev2 = 0;   /* p[-2] */
    seq_t p_prev1 = 1;   /* p[-1] */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = (seq_t)21;                       /* a0 */
        else
            ai = coeff[(size_t)((i - 1) % 6)];     /* a_i for i≥1 */

        seq_t p = ai * p_prev1 + p_prev2;         /* p[i] */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1; /* a(n) = p[n] */
}
