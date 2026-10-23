/*
 * A042035  Denominators of continued fraction convergents to sqrt(541).
 * https://oeis.org/A042035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 23, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(541).
   The algorithm uses the standard iteration for quadratic surds:
   starting from a0 = ⌊√541⌋ = 23, it generates the terms a_k of the
   periodic continued fraction and builds the convergents
   p_k / q_k via
     p_k = a_k * p_{k-1} + p_{k-2}
     q_k = a_k * q_{k-1} + q_{k-2}
   The sequence a(n) returned is q_n.  */
seq_t a(idx_t n) {
    const idx_t N   = 541;          /* the square‑free integer */
    const idx_t a0  = 23;           /* floor(sqrt(541)) */

    idx_t a = a0;   /* current partial quotient a_k */
    idx_t m = 0, d = 1;   /* auxiliary values for the continued fraction */

    /* initial convergents for k = -2 and -1 */
    seq_t p_prev2 = 0, p_prev1 = 1;
    seq_t q_prev2 = 1, q_prev1 = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* compute the k‑th convergent */
        seq_t p = (seq_t)a * p_prev1 + p_prev2;
        seq_t q = (seq_t)a * q_prev1 + q_prev2;
        if (k == n) return q;

        /* shift the window for the next step */
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;

        /* generate the next partial quotient a_{k+1} */
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
    }
    /* this point is never reached, but the compiler requires a return */
    return 0;
}
