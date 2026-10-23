/*
 * A008160  Coordination sequence T1 for Zeolite Code MER.
 * https://oeis.org/A008160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(15*m + k) = 420*m^2 + 56*k*m + C[k] where C[0] = 2 and for 1 ≤
 *            k < 15, C[k] = a(k) from the initial block
 * Range:     u8 n <= 11, u16 n <= 187, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special-case the first element which is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Constants C[0]..C[14] for the formula. */
    static const seq_t C[15] = {
        (seq_t)2,   /* C[0]  (used only for n > 0) */
        (seq_t)4,   /* C[1]  */
        (seq_t)9,   /* C[2]  */
        (seq_t)18,  /* C[3]  */
        (seq_t)32,  /* C[4]  */
        (seq_t)49,  /* C[5]  */
        (seq_t)69,  /* C[6]  */
        (seq_t)93,  /* C[7]  */
        (seq_t)121, /* C[8]  */
        (seq_t)153, /* C[9]  */
        (seq_t)189, /* C[10] */
        (seq_t)228, /* C[11] */
        (seq_t)270, /* C[12] */
        (seq_t)317, /* C[13] */
        (seq_t)368  /* C[14] */
    };

    idx_t m_idx = n / 15;
    idx_t k_idx = n % 15;

    seq_t m = (seq_t)m_idx;
    seq_t k = (seq_t)k_idx;

    /* Compute the quadratic part 420*m^2 + 56*k*m.  */
    seq_t term1 = (seq_t)420 * m * m;
    seq_t term2 = (seq_t)56  * k * m;

    return term1 + term2 + C[k_idx];
}
