/*
 * A042563  Denominators of continued fraction convergents to sqrt(810).
 * https://oeis.org/A042563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  q_{-1}=0, q_0=1.  
   For k≥1, let a_k be the k‑th continued‑fraction coefficient of √810  
   (a_0 = 28, a_1 = 2, …).  Then
          q_k = a_k * q_{k-1} + q_{k-2} .
   The sequence a(n) in OEIS A042563 is q_n (counting from n=0).  */
/* Initial values:  a(0)=1, a(1)=2.  */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;

    /* Constants for √810 */
    const idx_t D  = 810u;   /* radicand */
    const idx_t A0 = 28u;    /* floor(√810) */

    /* Continued fraction variables: m, d, a (small integers)   */
    idx_t m = 0u;
    idx_t d = 1u;
    idx_t a_coeff = A0;

    /* Denominator recurrence variables (big integers)   */
    seq_t q_prevprev = (seq_t)0;   /* q_{-1} */
    seq_t q_prev     = (seq_t)1;   /* q_0  */
    seq_t q_curr     = (seq_t)0;   /* placeholder for q_k */

    /* Iterate through the first n convergents */
    for (idx_t k = 1u; k <= n; ++k) {
        /* Update continued‑fraction coefficients for next k */
        m = d * a_coeff - m;
        d = (D - m * m) / d;
        a_coeff = (A0 + m) / d;

        /* Compute q_k = a_k * q_{k-1} + q_{k-2} */
        q_curr = (seq_t)a_coeff * q_prev + q_prevprev;

        /* Shift for next iteration */
        q_prevprev = q_prev;
        q_prev = q_curr;
    }

    return q_curr;
}
