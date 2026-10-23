/*
 * A042759  Denominators of continued fraction convergents to sqrt(910).
 * https://oeis.org/A042759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(910)
   Recurrence:
     q_{-1} = 0,  q_0 = 1
     For k ≥ 1:
       a_k = (k % 2 == 1) ? 6 : 60
       q_k = a_k * q_{k-1} + q_{k-2}
   The sequence a(n) is q_n. */
seq_t a(idx_t n)
{
    if (n == 0)          /* q_0 = 1 */
        return (seq_t)1;

    seq_t q_prev2 = (seq_t)0;   /* q_{-1} */
    seq_t q_prev1 = (seq_t)1;   /* q_0   */
    seq_t q_curr;
    idx_t k;

    for (k = 1; k <= n; ++k) {
        seq_t a_k = ((k & 1) ? (seq_t)6 : (seq_t)60);   /* 6 for odd k, 60 for even k */
        q_curr   = a_k * q_prev1 + q_prev2;
        q_prev2  = q_prev1;
        q_prev1  = q_curr;
    }
    return q_prev1;   /* q_n */
}
