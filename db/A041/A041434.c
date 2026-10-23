/*
 * A041434  Numerators of continued fraction convergents to sqrt(233).
 * https://oeis.org/A041434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of nth convergent to sqrt(233). */
/* Recurrence: let p_-1=1, p_-2=0. For n≥0,
   p_n = coeff(n)*p_{n-1} + p_{n-2}
   coeff(0)=15, coeff(n)=period[(n-1)%11] for n>0,
   where period = {3,1,3,1,1,1,1,3,1,3,30}.  The sequence a(n)=p_n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned char period[11] = {3,1,3,1,1,1,1,3,1,3,30};
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t p_i = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)15;
        else
            coeff = (seq_t)period[(i - 1) % 11];
        p_i = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_i;
    }
    return p_i;
}
