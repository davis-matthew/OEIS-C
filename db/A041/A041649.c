/*
 * A041649  Denominators of continued fraction convergents to sqrt(343).
 * https://oeis.org/A041649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(343).
   The continued fraction of sqrt(343) is
   [18; 1,1,11,1,5,3,1,17,1,3,5,1,11,1,1,36,…]  with period 16.
   The continuants q_n are computed from
        q_{-2}=1, q_{-1}=0,
        q_i = a_i * q_{i-1} + q_{i-2},
   where a_0 = 18 and a_i for i≥1 are given by the period above. */

seq_t a(idx_t n) {
    /* Period of the CF after the initial term 18. */
    static const unsigned period[] = {
        1, 1, 11, 1, 5, 3, 1, 17,
        1, 3, 5, 1, 11, 1, 1, 36
    };
    const idx_t period_len = 16u;

    seq_t q_prev_prev = 1; /* q_{-2} */
    seq_t q_prev      = 0;  /* q_{-1} */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = (seq_t)18;            /* a_0 */
        } else {
            ai = (seq_t)period[(i - 1) % period_len];
        }
        seq_t q_curr = ai * q_prev + q_prev_prev;
        q_prev_prev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;   /* q_n */
}
