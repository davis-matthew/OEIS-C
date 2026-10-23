/*
 * A042068  Numerators of continued fraction convergents to sqrt(558).
 * https://oeis.org/A042068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(558).
   Recurrence:  p[-2] = 0, p[-1] = 1;
                p[n] = a_n * p[n-1] + p[n-2];
   where a_n are the partial quotients of sqrt(558). */
 
static inline seq_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}
 
seq_t a(idx_t n)
{
    const idx_t D = 558u;               /* the number under the square root */

    /* base partial quotient (a0) */
    idx_t a0 = (idx_t)int_sqrt_u64(D);

    /* special case n == 0 */
    if (n == 0) {
        return (seq_t)a0;
    }

    /* continued‑fraction state for generating a_k */
    idx_t m  = 0;          /* m_0 */
    idx_t d  = 1;          /* d_0 */
    idx_t a_cur = a0;      /* a_0, will be updated in the loop */

    /* recurrence holders: p[-2] = 0, p[-1] = 1 */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p = 0;

    /* iterate from k = 0 up to n */
    for (idx_t k = 0; k <= n; ++k) {
        /* compute current numerator */
        p = (seq_t)a_cur * p_prev1 + p_prev2;

        if (k == n) {
            return p;
        }

        /* advance the continued‑fraction state to obtain a_{k+1} */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = ((idx_t)D - m_next * m_next) / d;
        idx_t a_next = ((idx_t)a0 + m_next) / d_next;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a_cur = a_next;

        /* shift numerator recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    /* unreachable return for completeness */
    return 0;
}
