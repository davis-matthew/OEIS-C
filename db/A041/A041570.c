/*
 * A041570  Numerators of continued fraction convergents to sqrt(303).
 * https://oeis.org/A041570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=17; a(n)=coeff[n]*a(n-1)+a(n-2),
   coeff[0]=17, coeff[1..] repeat {2,2,5,2,2,34}. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic continued fraction coefficients for sqrt(303) after the
       initial term: 2, 2, 5, 2, 2, 34. */
    static const unsigned int period[6] = {2, 2, 5, 2, 2, 34};
    const unsigned int period_len = 6;

    /* Convergent numerators: h_{-2}=0, h_{-1}=1 */
    seq_t h_prev2 = (seq_t)0;
    seq_t h_prev1 = (seq_t)1;

    /* Coefficient at step i: a_0=17, a_{i} = period[(i-1)%period_len] for i>=1 */
    unsigned int coeff;

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
            coeff = 17;
        else
            coeff = period[(i - 1) % period_len];

        seq_t h_curr = coeff * h_prev1 + h_prev2;

        h_prev2 = h_prev1;
        h_prev1 = h_curr;
    }

    return h_prev1;
}
