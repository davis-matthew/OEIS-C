/*
 * A041822  Numerators of continued fraction convergents to sqrt(432).
 * https://oeis.org/A041822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(432).
   The continued fraction of sqrt(432) is [20; overline{1,3,1,1,1,3,1,40}].
   Recurrence for the numerators p_k:
     p_{-2} = 0,  p_{-1} = 1,
     p_k   = a_k * p_{k-1} + p_{k-2}      (k ≥ 0)
   where a_0 = 20 and a_k for k≥1 follows the period above.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint8_t period[8] = {1, 3, 1, 1, 1, 3, 1, 40};

    seq_t p_minus2 = (seq_t)0;   /* p_{-2} */
    seq_t p_minus1 = (seq_t)1;   /* p_{-1} */
    seq_t p = (seq_t)0;          /* current numerator */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0)
            ak = (seq_t)20;
        else
            ak = (seq_t)period[(k - 1) % 8];

        p = ak * p_minus1 + p_minus2;

        p_minus2 = p_minus1;
        p_minus1 = p;
    }
    return p_minus1;
}
