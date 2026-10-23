/*
 * A041185  Denominators of continued fraction convergents to sqrt(103).
 * https://oeis.org/A041185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to √103.
   Recurrence: q_{-2}=1, q_{-1}=0, q_n = a_n·q_{n-1}+q_{n-2}
   Partial quotients: a_0=10, and for n≥1 a_n repeats
   6,1,2,1,1,9,1,1,2,1,6,20 (period 12). */

seq_t a(idx_t n)
{
    static const seq_t periodic[12] = {
        (seq_t)6, (seq_t)1, (seq_t)2, (seq_t)1,
        (seq_t)1, (seq_t)9, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)6, (seq_t)20
    };
    seq_t q_prev2 = (seq_t)1;  /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;  /* q_{-1} */
    seq_t q_curr;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        seq_t coeff = (i == 0) ? (seq_t)10
                               : periodic[(i - 1) % 12];
        q_curr = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_prev1;
}
