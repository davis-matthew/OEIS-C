/*
 * A042385  Denominators of continued fraction convergents to sqrt(719).
 * https://oeis.org/A042385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1.
   The sequence gives the denominators q_n of the convergents to sqrt(719).
   Let a_0 = floor(sqrt(719)) = 26 and the periodic part of the continued
   fraction be
     a_1 = 1, a_2 = 4, a_3 = 2, a_4 = 1, a_5 = 1, a_6 = 1, a_7 = 1, a_8 = 1,
     a_9 = 4, a_10 = 3, a_11 = 1, a_12 = 9, a_13 = 1, a_14 = 25, a_15 = 1,
     a_16 = 9, a_17 = 1, a_18 = 3, a_19 = 4, a_20 = 1, a_21 = 1, a_22 = 1,
     a_23 = 1, a_24 = 1, a_25 = 2, a_26 = 4, a_27 = 1, a_28 = 52,
   which repeats.  The denominators satisfy
     q_{-1} = 0, q_0 = 1,
     q_k   = a_k * q_{k-1} + q_{k-2}   for k ≥ 1.
   The function returns q_n. */

seq_t a(idx_t n) {
    /* Periodic continued fraction coefficients for sqrt(719) after a0. */
    static const idx_t period[28] = {
        1, 4, 2, 1, 1, 1, 1, 1,
        4, 3, 1, 9, 1, 25, 1, 9,
        1, 3, 4, 1, 1, 1, 1, 1,
        2, 4, 1, 52
    };
    const idx_t period_len = 28;

    if (n == 0) {
        return (seq_t)1;
    }

    seq_t q_prev2 = (seq_t)0; /* q_{-1} */
    seq_t q_prev1 = (seq_t)1; /* q_0   */

    for (idx_t i = 1; i <= n; ++i) {
        /* coefficient a_i : a_1..a_{period_len} repeat. */
        idx_t coeff_idx = (i - 1) % period_len;
        seq_t coeff = (seq_t)period[coeff_idx];
        seq_t q_curr = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_prev1;
}
