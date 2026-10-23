/*
 * A041398  Numerators of continued fraction convergents to sqrt(214).
 * https://oeis.org/A041398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 14 a(n) = coeff[(n-1) % 26] * a(n-1) + a(n-2) for n > 0
 *            where coeff =
 *            [1,1,1,2,3,1,4,9,1,1,5,3,14,3,5,1,1,9,4,1,3,2,1,1,1,28] This
 *            implements OEIS A041398: numerators of continued fraction
 *            convergents to sqrt(214)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Partial quotients of the continued fraction of sqrt(214) after the integer part */
static const unsigned char cf[26] = {
    1, 1, 1, 2, 3, 1, 4, 9, 1, 1, 5, 3,
    14, 3, 5, 1, 1, 9, 4, 1, 3, 2, 1, 1, 1, 28
};

seq_t a(idx_t n)
{
    seq_t p_nm2 = (seq_t)0;          /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;          /* p_{-1} */
    seq_t p_n   = (seq_t)0;          /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0U) {
            coeff = (seq_t)14;
        } else {
            coeff = (seq_t)cf[(i-1U) % 26U];
        }
        p_n = coeff * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_n;
}
