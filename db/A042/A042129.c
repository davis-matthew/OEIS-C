/*
 * A042129  Denominators of continued fraction convergents to sqrt(589).
 * https://oeis.org/A042129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042129: denominators of continued fraction convergents to sqrt(589)
   The continued fraction of sqrt(589) has period 40 with partial quotients:
   [24, 3, 1, 2, 2, 15, 1, 3, 9, 2, 4, 1, 11, 3, 6, 1, 1, 1, 1, 3,
    2, 3, 1, 1, 1, 1, 6, 3, 11, 1, 4, 2, 9, 3, 1, 15, 2, 2, 1, 3, 48]
   Denominators q_n satisfy the recurrence
       q_{-2} = 1,   q_{-1} = 0
       q_n   = a_n * q_{n-1} + q_{n-2}
   where a_n is the nth partial quotient (a_0 = 24).
*/

#include <stddef.h>

/* Periodic list of continued‑fraction partial quotients for sqrt(589). */
static const int period[40] = {
    3, 1, 2, 2, 15, 1, 3, 9, 2, 4,
    1, 11, 3, 6, 1, 1, 1, 1, 3, 2,
    3, 1, 1, 1, 1, 6, 3, 11, 1, 4,
    2, 9, 3, 1, 15, 2, 2, 1, 3, 48
};

/* Compute the nth denominator of the continued fraction convergents to sqrt(589). */
seq_t a(idx_t n) {
    /* Initial values for the recurrence:
       p_{-2}=0, p_{-1}=1   (not explicitly needed for the denominator)
       q_{-2}=1, q_{-1}=0
    */
    seq_t q_prev2 = 1;   /* q_{-2} */
    seq_t q_prev1 = 0;   /* q_{-1} */
    seq_t q_curr  = 0;   /* will hold q_n */

    /* Iterate from k=0 up to n inclusive */
    for (idx_t k = 0; k <= n; ++k) {
        /* Determine the k-th partial quotient a_k */
        seq_t a_k;
        if (k == 0) {
            a_k = (seq_t)24;   /* a_0 = floor(sqrt(589)) */
        } else {
            a_k = (seq_t)period[(k - 1) % 40];
        }

        /* Compute q_k = a_k * q_{k-1} + q_{k-2} */
        q_curr = a_k * q_prev1 + q_prev2;

        /* Shift for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    /* q_curr now holds q_n because loop ended with k==n */
    return q_curr;
}
