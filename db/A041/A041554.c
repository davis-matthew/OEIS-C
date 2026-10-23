/*
 * A041554  Numerators of continued fraction convergents to sqrt(295).
 * https://oeis.org/A041554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(295)
   a(0) = 17
   a(1) = 86
   For n >= 2: a(n) = aCF[n-1] * a(n-1) + a(n-2)
   where aCF[i] is the i‑th partial quotient of the continued fraction
   expansion of sqrt(295). */
seq_t a(idx_t n)
{
    /* Continued fraction constants for sqrt(295) */
    const seq_t D     = (seq_t)295;   /* radicand */
    const seq_t a0    = (seq_t)17;    /* floor(sqrt(295)) */

    /* Continued fraction state variables */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_k = a0;   /* current partial quotient */

    /* Convergent numerators: P_{-2} = 0, P_{-1} = 1 */
    seq_t P_prev2 = (seq_t)0;
    seq_t P_prev1 = (seq_t)1;
    seq_t Pk = (seq_t)0;

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        /* Compute current numerator */
        Pk = a_k * P_prev1 + P_prev2;
        /* Shift for next step */
        P_prev2 = P_prev1;
        P_prev1 = Pk;

        if (k < n)
        {
            /* Advance continued‑fraction state to next term */
            m = d * a_k - m;
            d = (D - m * m) / d;
            a_k = (a0 + m) / d;
        }
    }
    return Pk;
}
