/*
 * A041074  Numerators of continued fraction convergents to sqrt(44).
 * https://oeis.org/A041074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041074: numerators of the convergents to sqrt(44) in its continued fraction.
   The continued fraction is [6; overline{1,1,1,2,1,1,1,12}].
   Recurrence: let p_{-2}=0, p_{-1}=1.
   For k>=0,
        a_k = 6 if k==0 else coeff[(k-1)&7]
        p_k = a_k * p_{k-1} + p_{k-2}
   The function returns p_n. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t coeff[8] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)12
    };
    seq_t prev2 = (seq_t)0;  /* p_{-2} */
    seq_t prev1 = (seq_t)1;  /* p_{-1} */
    seq_t pk    = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = (k == 0) ? (seq_t)6
                            : coeff[(size_t)((k - 1) & 7)];
        pk = ak * prev1 + prev2;    /* p_k = a_k * p_{k-1} + p_{k-2} */
        prev2 = prev1;
        prev1 = pk;
    }
    return pk;
}
