/*
 * A042934  Numerators of continued fraction convergents to sqrt(999).
 * https://oeis.org/A042934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(999)
   Computed by standard convergent recurrence:
     h_{-1}=1, h_0=a0
     h_n = a_n * h_{n-1} + h_{n-2}
   where a_n are the partial quotients of sqrt(999). */

seq_t a(idx_t n)
{
    /* Constants for sqrt(999) */
    const seq_t a0 = 31;            /* floor(sqrt(999)) */
    const idx_t Dint = 999;         /* value of D */
    const seq_t D = (seq_t)Dint;

    if (n == 0) return a0;

    /* Continued‑fraction state variables */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cf = a0;

    /* Convergent numerators */
    seq_t h_prev2 = 1;             /* h_{-1} */
    seq_t h_prev1 = a0;            /* h_{0} */

    /* Denominators (kept for completeness, not used afterwards) */
    seq_t k_prev2 = 0;
    seq_t k_prev1 = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* Next partial quotient a_i */
        m = d * a_cf - m;
        seq_t diff = D - m * m;
        d = diff / d;
        a_cf = (a0 + m) / d;

        /* Update convergents */
        seq_t h_new = a_cf * h_prev1 + h_prev2;
        seq_t k_new = a_cf * k_prev1 + k_prev2;

        h_prev2 = h_prev1;
        h_prev1 = h_new;
        k_prev2 = k_prev1;
        k_prev1 = k_new;
    }

    return h_prev1;
}
