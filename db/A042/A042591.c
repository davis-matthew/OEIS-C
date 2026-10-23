/*
 * A042591  Denominators of continued fraction convergents to sqrt(824).
 * https://oeis.org/A042591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = denominator of the nth convergent of the continued
   fraction expansion of sqrt(824).  The continued fraction has the form
   [28; 1,2,2,1,1,6,1,1,2,2,1,56] repeating the periodic part of length
   12.  The recurrence for the denominator q_n is:
     q_(-2) = 1, q_(-1) = 0
     q_k   = a_k * q_(k-1) + q_(k-2)   for k >= 0
   where a_0 = 28 and for k > 0, a_k is the kth element of the periodic
   part with indices modulo 12.  The function below implements this
   recurrence iteratively, using only integer arithmetic. */

seq_t a(idx_t n)
{
    static const int period_int[12] = {1,2,2,1,1,6,1,1,2,2,1,56};

    seq_t p_pp = (seq_t)0;   /* p_{k-2} */
    seq_t p_p  = (seq_t)1;   /* p_{k-1} */
    seq_t q_pp = (seq_t)1;   /* q_{k-2} */
    seq_t q_p  = (seq_t)0;   /* q_{k-1} */
    seq_t p_k, q_k;
    seq_t a_k;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            a_k = (seq_t)28;
        } else {
            a_k = (seq_t)period_int[(k - 1) % 12];
        }

        p_k = a_k * p_p + p_pp;
        q_k = a_k * q_p + q_pp;

        p_pp = p_p;  p_p = p_k;
        q_pp = q_p;  q_p = q_k;
    }

    return q_k;
}
