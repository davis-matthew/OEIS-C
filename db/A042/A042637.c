/*
 * A042637  Denominators of continued fraction convergents to sqrt(848).
 * https://oeis.org/A042637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(848).
   Recurrence:
       q_{-2} = 1, q_{-1} = 0
       For n ≥ 0 let a_n be the n‑th partial quotient:
           a_0 = 29,
           a_k = period[(k-1)%6], k>0
           where period = {8, 3, 3, 3, 8, 58}
       Then q_n = a_n * q_{n-1} + q_{n-2}
   The sequence a(n) is simply q_n. */

seq_t a(idx_t n)
{
    static const unsigned int period[6] = {8, 3, 3, 3, 8, 58};
    seq_t q_prev2 = 1;   /* q_{-2} */
    seq_t q_prev1 = 0;   /* q_{-1} */
    seq_t q_curr;

    for (idx_t i = 0; i <= n; ++i) {
        unsigned int ai;
        if (i == 0)
            ai = 29;
        else
            ai = period[(i - 1) % 6];
        q_curr = (seq_t)ai * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_curr;
}
