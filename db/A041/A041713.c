/*
 * A041713  Denominators of continued fraction convergents to sqrt(376).
 * https://oeis.org/A041713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 24, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041713: a(n) = denominator of the n‑th convergent to sqrt(376). */
/* Recurrence: a(0)=1, a(1)=2, a(k)=c_k*a(k-1)+a(k-2) where c_k follows a period of 16:
   c = {2,1,1,3,1,2,2,4,2,2,1,3,1,1,2,38}                       */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint16_t period[16] = {
        2, 1, 1, 3, 1, 2, 2, 4,
        2, 2, 1, 3, 1, 1, 2, 38
    };

    seq_t prev2 = 0;   // q_{k-2}
    seq_t prev1 = 1;   // q_{k-1}

    for (idx_t k = 1; k <= n; ++k)
    {
        uint16_t coeff_val = period[(size_t)((k - 1) % 16)];
        seq_t coeff = (seq_t)coeff_val;
        seq_t curr = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
