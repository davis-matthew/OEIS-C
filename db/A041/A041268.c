/*
 * A041268  Numerators of continued fraction convergents to sqrt(147).
 * https://oeis.org/A041268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(147).
   Recurrence for the numerator p_k:
       p(-2) = 0, p(-1) = 1
       p(k)   = a_k * p(k-1) + p(k-2)
   with partial quotients a_k defined by:
       a_0 = 12
       for k ≥ 1, a_k = 8 if k is odd, else 24
   This generates a(0)=12, a(1)=97, a(2)=2340, … */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev2 = 0;   /* p(k-2)  */
    seq_t prev1 = 1;   /* p(k-1)  */
    seq_t curr;
    idx_t k;

    for (k = 0; k <= n; ++k)
    {
        seq_t ak;
        if (k == 0)
            ak = 12;
        else if (k & 1)  /* odd k */
            ak = 8;
        else
            ak = 24;

        curr = ak * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;      /* p(n) */
}
