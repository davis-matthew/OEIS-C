/*
 * A042534  Numerators of continued fraction convergents to sqrt(796).
 * https://oeis.org/A042534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(796).
   The recurrence is
        p_-2 = 0,  p_-1 = 1
        p_k   = a_k * p_{k-1} + p_{k-2}
   with a_0 = 28 and for k ≥ 1
        a_k = period[(k-1) mod 44]
   where the period is
        4,1,2,5,1,10,2,3,1,6,
        3,1,1,1,1,2,4,1,2,1,
        18,14,18,1,2,1,4,2,1,1,
        1,1,3,6,1,3,2,10,1,5,
        2,1,4,56. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued‑fraction partial quotients of sqrt(796). */
    static const seq_t period[] = {
        4, 1, 2, 5, 1, 10, 2, 3, 1, 6,
        3, 1, 1, 1, 1, 2, 4, 1, 2, 1,
        18, 14, 18, 1, 2, 1, 4, 2, 1, 1,
        1, 1, 3, 6, 1, 3, 2, 10, 1, 5,
        2, 1, 4, 56
    };
    static const seq_t a0 = 28;
    const size_t P = sizeof(period) / sizeof(period[0]);

    seq_t p_im2 = 0;   /* p_{k-2} */
    seq_t p_im1 = 1;   /* p_{k-1} */
    seq_t pk    = 0;   /* current p_k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = a0;
        } else {
            size_t idx = (size_t)((k - 1) % P); /* period index for a_k */
            ak = period[idx];
        }
        pk = ak * p_im1 + p_im2;
        p_im2 = p_im1;
        p_im1 = pk;
    }
    return pk;
}
