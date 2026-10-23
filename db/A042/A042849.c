/*
 * A042849  Denominators of continued fraction convergents to sqrt(955).
 * https://oeis.org/A042849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A042849
   Denominators of continued fraction convergents to sqrt(955).
   The continued fraction of sqrt(955) has period 16:
   [30; 1, 9, 3, 6, 1, 1, 5, 12, 5, 1, 1, 6, 3, 9, 1, 60]
   The n‑th term a(n) is the denominator q_n of the convergent
   obtained by
         q(-2)=1;  q(-1)=0
         q(n) = a(n)*q(n-1) + q(n-2)
   where a(0)=30 and for n≥1 the coefficients repeat with period 16 as
   listed above.
*/

#include <stdlib.h>

static inline seq_t cf_coeff(idx_t n)
{
    if (n == 0)
        return (seq_t)30;
    static const unsigned int coeff[16] =
        {1,9,3,6,1,1,5,12,5,1,1,6,3,9,1,60};
    unsigned int k = (unsigned int)((n - 1) % 16);
    return (seq_t)coeff[k];
}

seq_t a(idx_t n)
{
    seq_t q_prev2 = (seq_t)1;   /* q(-2) */
    seq_t q_prev1 = (seq_t)0;   /* q(-1) */
    seq_t q_cur;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff = cf_coeff(i);
        q_cur = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_cur;
    }
    return q_prev1;
}
