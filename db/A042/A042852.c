/*
 * A042852  Numerators of continued fraction convergents to sqrt(957).
 * https://oeis.org/A042852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042852 – Numerators of convergents to sqrt(957)
 *
 *  a(n) is the numerator of the nth convergent of sqrt(957).
 *  It satisfies the linear recurrence
 *      a(n) = 29698*a(n-6) - a(n-12)   for n ≥ 12
 *  with initial values
 *      a(0)=30,  a(1)=31,  a(2)=464,  a(3)=959,
 *      a(4)=13890, a(5)=14849.
 *
 *  The implementation below builds the convergents directly
 *  using the standard continued‑fraction algorithm for sqrt(D)
 *  (D = 957).  This avoids any hard‑coded table and works for
 *  arbitrary n (up to the limits of the underlying integer type).
 *
 *  No dynamic memory allocation, floating point,
 *  or external helper functions are used.  All arithmetic
 *  is performed with the allowed operators.
 *
 *  Complexity: O(n).  Memory usage: O(1).
 */

seq_t a(idx_t n)
{
    /* constant part of sqrt(957) */
    const idx_t D = 957;          /* D > 0, D is not a perfect square */
    const idx_t sqrtD = 30;       /* floor(sqrt(957)) */

    /* variables for the continued‑fraction expansion (all fit in idx_t) */
    idx_t m = 0;          /* m_k */
    idx_t d = 1;          /* d_k */
    idx_t a_k = sqrtD;    /* a_k (partial quotient) */

    /* previous two convergent numerators:
     * p_-2 = 0,  p_-1 = 1   for the recurrence p_k = a_k * p_{k-1} + p_{k-2} */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;

    /* produce the first numerator a(0) */
    seq_t p_cur = (seq_t)a_k * p_prev1 + p_prev2;   /* = sqrtD */
    if (n == 0) return p_cur;

    /* shift for the next step */
    p_prev2 = p_prev1;
    p_prev1 = p_cur;

    /* iterate to compute up to a(n) */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* next partial quotient (using integers only) */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (sqrtD + m_next) / d_next;   /* floor((a0 + m_next)/d_next) */

        /* update state for next step */
        m = m_next;
        d = d_next;
        a_k = a_next;

        /* compute the k‑th convergent numerator */
        p_cur = (seq_t)a_k * p_prev1 + p_prev2;

        if (k == n) return p_cur;

        /* shift for subsequent iterations */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    /* The loop always returns inside; this line is only a safety net. */
    return (seq_t)0;
}
