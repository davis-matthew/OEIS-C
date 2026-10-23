/*
 * A041447  Denominators of continued fraction convergents to sqrt(239).
 * https://oeis.org/A041447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A041447: denominators of continued‑fraction convergents to sqrt(239).
 *
 * The denominators Q_k of the convergents satisfy
 *      Q_{-1} = 1,  Q_0 = 0,
 *      Q_{k+1} = a_k * Q_k + Q_{k-1},
 * where a_k are the continued‑fraction coefficients of sqrt(239).
 * The required term is a(n) = Q_{n+1}.
 *
 * This implementation generates the continued‑fraction coefficients
 * by the standard modular algorithm and computes the sequence on the
 * fly, avoiding any hard‑coded tables and using only integer arithmetic.
 */
seq_t a(idx_t n)
{
    /* continued‑fraction data for sqrt(239) */
    unsigned long long m   = 0;          /* m_{0}   */
    unsigned long long d   = 1;          /* d_{0}   */
    const unsigned long long a0 = 15;    /* floor(sqrt(239)) */
    unsigned long long a   = a0;         /* a_{0}   */

    /* Fibonacci‑type recurrence for denominators */
    seq_t q_prevprev = (seq_t)1;   /* Q_{-1} */
    seq_t q_prev    = (seq_t)0;   /* Q_0    */
    seq_t q_curr    = 0;           /* will hold Q_{k+1} */

    for (idx_t i = 0; i <= n; ++i) {
        /* compute Q_{i+1} = a_i * Q_i + Q_{i-1} */
        q_curr = q_prev * (seq_t)a + q_prevprev;

        /* shift indices for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_curr;

        /* generate next continued‑fraction coefficient a_{i+1} */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (239 - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
    return q_curr;
}
