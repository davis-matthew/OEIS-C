/*
 * A041625  Denominators of continued fraction convergents to sqrt(331).
 * https://oeis.org/A041625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 22, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(331). */
/* Recurrence: q_{-2}=1, q_{-1}=0; q_n = a_n*q_{n-1} + q_{n-2}. */
/* Partial quotients: a0 = 18, then periodic of length 34: */
/* 5,5,1,6,2,3,1,1,2,1,2,1,11,2,1,1,17,1,1,2,11,1,2,1,2,1,1,3,2,6,1,5,5,36 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const unsigned int period[34] = {
        5,5,1,6,2,3,1,1,2,1,2,1,11,2,1,1,17,1,1,2,
        11,1,2,1,2,1,1,3,2,6,1,5,5,36
    };

    seq_t prev2 = 1;   /* q_{-2} */
    seq_t prev1 = 0;   /* q_{-1} */
    seq_t curr = 0;

    /* Iterate from n = 0 up to the requested index. */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)18;           /* a0 */
        } else {
            coeff = (seq_t)period[(i - 1) % 34u]; /* a_i for i>=1 */
        }
        curr = coeff * prev1 + prev2;     /* q_i */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
