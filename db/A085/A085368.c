/*
 * A085368  Sum of numerators and denominators of convergents to e.
 * https://oeis.org/A085368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007676(n) + A007677(n)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n == 0 we return the first convergent 2/1.  */
    if (n == 0)
        return (seq_t)2 + (seq_t)1;

    /* Fibonacci‑like recurrence for convergents to e.
       p_{-2} = 0, p_{-1} = 1
       q_{-2} = 1, q_{-1} = 0
       a_0 = 2, a_i (i>0) = 1 except every third term:
       a_{3k+2} = 2*(k+1)
    */
    seq_t p_prev2 = 0, p_prev1 = 1;
    seq_t q_prev2 = 1, q_prev1 = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = (seq_t)2;
        else if (i % 3 == 2)
            ai = (seq_t)(2 * ((idx_t)(i / 3) + 1));
        else
            ai = (seq_t)1;

        seq_t p  = ai * p_prev1 + p_prev2;
        seq_t q  = ai * q_prev1 + q_prev2;

        if (i == n)
            return p + q;

        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;
    }

    /* Should never reach here */
    return 0;
}
