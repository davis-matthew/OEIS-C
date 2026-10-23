/*
 * A042599  Denominators of continued fraction convergents to sqrt(828).
 * https://oeis.org/A042599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042599:  Denominators of continued fraction convergents to sqrt(828)
    The partial quotients of sqrt(828) repeat with period 6 after the
    initial term 28:  [28, 1, 3, 2, 3, 1, 56] , then 1,3,2,3,1,56, ...
    The denominators q_n of the convergents satisfy
         q_{-2} = 1,  q_{-1} = 0
         q_n   = a_n*q_{n-1} + q_{n-2}
    where a_n is the nth partial quotient described above.
*/

#include <stdint.h>          /* for uintptr_t definitions; seq_t/idx_t are supplied elsewhere */

seq_t a(idx_t n) {
    /*  q_{-2} = 1,  q_{-1} = 0  */
    seq_t q_prev2 = 1;    /* q_{-1} */
    seq_t q_prev1 = 0;    /* q_{-2} */
    seq_t q_curr;

    /*  periodic part of the continued fraction after the initial 28  */
    static const seq_t period[6] = {1, 3, 2, 3, 1, 56};

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0) {
            a_k = 28;
        } else {
            /* cycle over the 6-element period */
            a_k = period[(k - 1) % 6];
        }
        q_curr = a_k * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_curr;   /* q_n  */
}
