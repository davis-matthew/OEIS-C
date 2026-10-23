/*
 * A042809  Denominators of continued fraction convergents to sqrt(935).
 * https://oeis.org/A042809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 47, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(935).
   Recurrence: a(0) = 1, a(1) = 1.
   Let coeff(n) = [1, 1, 2, 1, 2, 1, 1, 60] (period 8) indexed from 1.
   Then for n >= 1:
       a(n) = coeff(n) * a(n-1) + a(n-2)
   This follows from the continued‑fraction recurrence for sqrt(935). */

seq_t a(idx_t n)
{
    /* Short‑cycle of partial quotients a_k for k >= 1 */
    static const int a_period[8] = {1, 1, 2, 1, 2, 1, 1, 60};

    /* q_{-1} = 0, q_0 = 1  (denominator of the 0‑th convergent) */
    seq_t q_prev2 = 0;   /* q_{i-2} */
    seq_t q_prev1 = 1;   /* q_{i-1} */

    if (n == 0)
        return q_prev1;  /* a(0) = 1 */

    /* Iterate the recurrence up to the requested index n */
    for (idx_t i = 1; i <= n; ++i) {
        int coeff = a_period[(int)((i - 1) % 8)];   /* periodic coefficients */
        seq_t q = ((seq_t)coeff * q_prev1) + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1; /* a(n) */
}
