/*
 * A041985  Denominators of continued fraction convergents to sqrt(515).
 * https://oeis.org/A041985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(515)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses recurrence:
   p_{-2}=0, p_{-1}=1, q_{-2}=1, q_{-1}=0
   p_n = a_n * p_{n-1} + p_{n-2}
   q_n = a_n * q_{n-1} + q_{n-2}
   with a_0 = 22 and periodic partial quotients a_{k>=1} of period 10:
   [1,2,3,1,3,1,3,2,1,44].
   The denominator q_n is seq_t a(n). */
seq_t a(idx_t n)
{
    /* partial quotients after the initial term 22 */
    static const int period[10] = {1, 2, 3, 1, 3, 1, 3, 2, 1, 44};

    /* Base cases: a(0)=1, a(1)=1 */
    if (n == 0 || n == 1)
        return 1;

    /* Recurrence initialization: q_{-2}=1, q_{-1}=0 */
    seq_t q_prev2 = 1;
    seq_t q_prev1 = 0;
    seq_t q_curr  = 0;

    /* Iterate from k=0 to n */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k = (k == 0) ? (seq_t)22
                             : (seq_t)period[(k - 1) % 10];
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        q_curr = a_k * q_prev1 + q_prev2;
        /* Shift for next round */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_curr;
}
