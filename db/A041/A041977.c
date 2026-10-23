/*
 * A041977  Denominators of continued fraction convergents to sqrt(511).
 * https://oeis.org/A041977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041977: Denominators of continued fraction convergents to sqrt(511)
   Recurrence:
   Let b_0 = 22 and for n ≥ 1 let
   b_n = period[(n-1) mod 20] where
       period = {1,1,1,1,6,1,14,4,1,21,1,4,14,1,6,1,1,1,1,44}.
   Set q_-2 = 1, q_-1 = 0.
   For n ≥ 0:  q_n = b_n * q_{n-1} + q_{n-2}.
   Then a(n) = q_n. */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Partial quotients period after a_0 = 22 */
    static const seq_t period[20] = {
        1, 1, 1, 1, 6,
        1,14, 4, 1,21,
        1, 4,14, 1, 6,
        1, 1, 1, 1,44
    };

    seq_t q_prev2 = (seq_t)1; /* q_-2 */
    seq_t q_prev1 = (seq_t)0; /* q_-1 */

    for (idx_t i = (idx_t)0; i <= n; ++i) {
        seq_t bi = (i == (idx_t)0) ? (seq_t)22 : period[(i - (idx_t)1) % (idx_t)20];
        seq_t q_i = bi * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_i;
    }
    return q_prev1; /* q_n */
}
