/*
 * A042388  Numerators of continued fraction convergents to sqrt(721).
 * https://oeis.org/A042388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(721)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 21, u64 n <= 43, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0 ≤ n, a(0)=26, a(1)=27, a(2)=161, ... as in the OEIS sequence
   A042388.  The recurrence for convergents is implemented directly. */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    /* The continued fraction of sqrt(721).  a0 = ⌊√721⌋ = 26. */
    const unsigned long long N  = 721ULL;
    const unsigned long long a0 = 26ULL;

    if (n == 0) {
        return (seq_t)a0;
    }

    /* State of the continued‑fraction generation */
    unsigned long long m = 0ULL;          /* m_k   */
    unsigned long long d = 1ULL;          /* d_k   */
    unsigned long long a = a0;            /* a_k   */

    /* Convergent numerators: p_{-1}=1, p_0=a0 */
    seq_t p_prevprev = 1;                 /* p_{k-2} */
    seq_t p_prev     = (seq_t)a0;        /* p_{k-1} */

    /* Iterate to the requested index */
    for (idx_t k = 1; k <= n; ++k) {
        /* Next triple via the standard square‑root algorithm */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (N - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* Update the numerator */
        seq_t p_curr = a * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    return p_prev;
}
