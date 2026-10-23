/*
 * A042797  Denominators of continued fraction convergents to sqrt(929).
 * https://oeis.org/A042797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Denominators of the continued‑fraction convergents to √929.
   Let a0 = ⌊√929⌋ = 30 and let (a1, a2, …) be the repeating
   coefficients of the continued fraction for √929.  The
   denominators q_n satisfy  
          q0 = 1,  q−1 = 0,
          qn = an · qn−1 + qn−2   (n ≥ 1).  
   Then a(n) = qn  for n ≥ 0.
*/

#include <stdlib.h>

static seq_t *period = NULL;
static idx_t period_len = 0;

/* Compute the period of the continued fraction for √929.  */
static void compute_period(void)
{
    if (period_len != 0)
        return;

    const int D   = 929;
    const int a0  = 30;       /* floor(sqrt(929)) */
    int m   = 0;
    int d   = 1;
    int a   = a0;

    idx_t cap = 16;
    period = (seq_t*)malloc(cap * sizeof(seq_t));
    period_len = 0;

    while (1) {
        int m_next = d * a - m;
        int d_next = (D - m_next * m_next) / d;
        int a_next = (a0 + m_next) / d_next;

        if (period_len == cap) {
            cap <<= 1;
            period = (seq_t*)realloc(period, cap * sizeof(seq_t));
        }
        period[period_len++] = (seq_t)a_next;

        if (a_next == 2 * a0)   /* last term of the period */
            break;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;      /* q0 = 1 */

    compute_period();

    seq_t q_prev2 = (seq_t)0;  /* q−1 = 0 */
    seq_t q_prev1 = (seq_t)1;  /* q0  = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t coeff = period[(k - 1) % period_len];
        seq_t qk    = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = qk;
    }
    return q_prev1;           /* qn = a(n) */
}
