/*
 * A041831  Denominators of continued fraction convergents to sqrt(436).
 * https://oeis.org/A041831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): denominators of convergents to sqrt(436) */
/* Uses the standard recurrence q_k = a_k*q_{k-1} + q_{k-2} with */
/* a_0 = 20, a_1..a_{30} periodic: 1,7,2,1,1,1,13,3,2,2,5,1,1,4,10,4,1,1,5,2,2,3,13,1,1,1,2,7,1,40 */

seq_t a(idx_t n) {
    /* Periodic part of the continued fraction, length 30 */
    static const idx_t cyc[30] = {
        1, 7, 2, 1, 1, 1, 13, 3, 2, 2,
        5, 1, 1, 4, 10, 4, 1, 1, 5, 2,
        2, 3, 13, 1, 1, 1, 2, 7, 1, 40
    };

    /* q_-2 = 1, q_-1 = 0  (initial state for the recurrence) */
    seq_t q_prev2 = 1;  /* q_{-2} */
    seq_t q_prev1 = 0;  /* q_{-1} */
    seq_t q_cur   = 0;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t term = (i == 0) ? 20 : cyc[(i-1) % 30];
        q_cur = (seq_t)term * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_cur;
    }
    return q_cur;
}
