/*
 * A041322  Numerators of continued fraction convergents to sqrt(175).
 * https://oeis.org/A041322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numerators of convergents to sqrt(175).
   Recurrence: a(0)=13, a(1)=53,
   a(n) = q_n * a(n-1) + a(n-2),
   where q_n are the continued‑fraction partial quotients of sqrt(175):
   13, 4, 2, 1, 2, 4, 26, 4, 2, 1, 2, 4, 26, … */

seq_t a(idx_t n)
{
    /* Handle the initial term explicitly */
    if (n == 0) return (seq_t)13;

    /* Constants for sqrt(175) */
    const unsigned long long N   = 175ULL;   /* target number */
    const unsigned long long a0  = 13ULL;    /* floor(sqrt(175)) */

    /* State variables for continued fraction computation */
    unsigned long long m_prev = 0ULL;        /* m_{k-1} */
    unsigned long long d_prev = 1ULL;        /* d_{k-1} */
    unsigned long long a_prev = a0;          /* a_{k-1} (initially a0) */

    /* Convergent numerators */
    seq_t p_prev2 = (seq_t)1;      /* p_{-1} */
    seq_t p_prev1 = (seq_t)13;     /* p_0 */

    /* Iterate through partial quotients up to the requested index */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next m, d, and partial quotient q = a_i */
        unsigned long long m_cur = d_prev * a_prev - m_prev;
        unsigned long long d_cur = (N - m_cur * m_cur) / d_prev;
        unsigned long long q_cur = (a0 + m_cur) / d_cur;

        /* Update convergent numerator: p_i = q_cur * p_{i-1} + p_{i-2} */
        seq_t p_cur = (seq_t)q_cur * p_prev1 + p_prev2;

        /* Shift state for the next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
        m_prev = m_cur;
        d_prev = d_cur;
        a_prev = q_cur;
    }

    return p_prev1;   /* p_n is the numerator of the n-th convergent */
}
