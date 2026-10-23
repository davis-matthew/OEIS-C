/*
 * A015275  Gaussian binomial coefficient [ n,3 ] for q = -7.
 * https://oeis.org/A015275
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implements the Gaussian binomial coefficient [n,3] for q = -7.
 * Recurrence:  a(0)=a(1)=a(2)=0,
 *                a(n) = a(n-1) + (-7)^(n-3) * [n-1,2]      (n>=3)
 * where [k,2] is tracked simultaneously.
 */

seq_t a(idx_t n)
{
    if (n < 3)    // behaviour below offset is irrelevant
        return (seq_t)0;

    const seq_t q = -((seq_t)7);

    /* previous values of Gaussian binomials:
     * a1 = [n,1], a2 = [n,2], a3 = [n,3] */
    seq_t prev_a1 = (seq_t)0;
    seq_t prev_a2 = (seq_t)0;
    seq_t prev_a3 = (seq_t)0;

    /* powers of q:
     * pow_q_n1 = q^(n-1), pow_q_n2 = q^(n-2), pow_q_n3 = q^(n-3)  */
    seq_t pow_q_n1 = (seq_t)1;   /* q^0 for n=1 */
    seq_t pow_q_n2 = (seq_t)1;   /* placeholder, used from n>=2 */
    seq_t pow_q_n3 = (seq_t)1;   /* placeholder, used from n>=3 */

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* new values based on previous ones */
        seq_t new_a1 = prev_a1 + pow_q_n1;                        /* [i,1] */
        seq_t new_a2 = prev_a2;
        if (i >= (idx_t)2)
            new_a2 += pow_q_n2 * prev_a1;                         /* [i,2] */
        seq_t new_a3 = prev_a3;
        if (i >= (idx_t)3)
            new_a3 += pow_q_n3 * prev_a2;                         /* [i,3] */

        /* update previous values for next iteration */
        prev_a1 = new_a1;
        prev_a2 = new_a2;
        prev_a3 = new_a3;

        /* advance powers of q for the next step */
        pow_q_n1 = pow_q_n1 * q;                                  /* q^(i)   */
        if (i >= (idx_t)2)
            pow_q_n2 = pow_q_n2 * q;                              /* q^(i-1) */
        if (i >= (idx_t)3)
            pow_q_n3 = pow_q_n3 * q;                              /* q^(i-2) */
    }

    return prev_a3;                                               /* a(n)  */
}
