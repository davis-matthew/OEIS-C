/*
 * A042184  Numerators of continued fraction convergents to sqrt(617).
 * https://oeis.org/A042184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to √617.
   The nth term is the denominator of the (n+1)-th convergent
   of the simple continued fraction of √617.
   Recurrence:
     q_{-1}=0, q_0=1,
     q_k = a_k·q_{k-1} + q_{k-2}   (k ≥ 1)
   Where
     a_0 = ⌊√617⌋ = 24,
     a_k = ⌊(a_0 + m_{k-1}) / d_{k-1}⌋,
     m_k   = a_k·d_{k-1} - m_{k-1},
     d_k   = (617 - m_k²) / d_{k-1}.
   The function returns q_{n+1} for n ≥ 0. */

seq_t a(idx_t n)
{
    const seq_t D   = (seq_t)617;          /* D = 617   */
    const seq_t a0  = (seq_t)24;           /* a0 = floor(sqrt(D)) */

    /* The trivial case n = 0 gives q1 = 24 directly. */
    if (n == 0) return a0;

    /* Initial continued‑fraction state for k = 0. */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;

    /* a1 explicitly, then advance state to k = 1. */
    seq_t a_k = a0;
    seq_t m_next = a_k * d - m;                 /* m1   */
    seq_t d_next = (D - m_next * m_next) / d;   /* d1   */
    m = m_next;  d = d_next;

    /* Convergents: q_{-1}=0, q0=1, so q1 = a1·q0 + q_{-1} = a1. */
    seq_t q_prev = (seq_t)1;    /* q0  */
    seq_t q_curr = a_k;         /* q1  */

    /* Compute q_{n+1} using the recurrence. */
    for (idx_t k = 2; k <= n + 1; ++k) {
        a_k = (a0 + m) / d;                     /* a_k   */
        m_next = a_k * d - m;                   /* m_k   */
        d_next = (D - m_next * m_next) / d;     /* d_k   */

        seq_t q_next = a_k * q_curr + q_prev;   /* q_k   */

        q_prev = q_curr;
        q_curr = q_next;

        m = m_next;
        d = d_next;
    }

    return q_curr;   /* q_{n+1} */
}
