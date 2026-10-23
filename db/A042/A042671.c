/*
 * A042671  Denominators of continued fraction convergents to sqrt(865).
 * https://oeis.org/A042671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(865)
   a(0) = 1.
   For n ≥ 1, let partial quotients a_n be given by the period
   [2,2,3,3,1,1,1,2,6,6,2,1,1,1,3,3,2,2,58] (length 19).
   The convergent denominators q_n satisfy
       q_n = a_n * q_{n-1} + q_{n-2},   with q_{-1}=0, q_0=1.
   The function returns q_n. */

seq_t a(idx_t n)
{
    /* Period of partial quotients a_n for n >= 1 */
    static const int period[] = {
        2, 2, 3, 3, 1, 1, 1, 2, 6, 6, 2,
        1, 1, 1, 3, 3, 2, 2, 58
    };
    static const int L = 19;           /* length of the period */

    if (n == 0)
        return (seq_t)1;   /* q_0 */

    /* q_{-1} = 0, q_0 = 1 */
    seq_t q_prev2 = 0;
    seq_t q_prev1 = 1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_i = (seq_t)period[(i - 1) % L];
        seq_t q = a_i * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
