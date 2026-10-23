/*
 * A041030  Numerators of continued fraction convergents to sqrt(20).
 * https://oeis.org/A041030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041030: numerator of the n‑th convergent of √20.
   Continued fraction of √20 is [4; overline{2,8}].
   Let p_{-2}=0, p_{-1}=1. For k≥0 set
        a_k = 4 if k==0 else (k%2==1 ? 2 : 8);
   Then   p_k = a_k * p_{k-1} + p_{k-2}.
   This function returns p_n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0;          /* p_{k-2}   */
    seq_t p_prev1 = 1;          /* p_{k-1}   */
    seq_t result = 0;           /* p_k       */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k = (k == 0) ? 4 : ( (k & 1) ? 2 : 8 );
        result = a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = result;
    }
    return result;
}
