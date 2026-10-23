/*
 * A042072  Numerators of continued fraction convergents to sqrt(560).
 * https://oeis.org/A042072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 23.
   For n ≥ 1 let the multipliers c_n cycle 1,1,1,46.
   Then a(n) = c_n * a(n‑1) + a(n‑2).
   These a(n) are the numerators of the convergents to √560. */
seq_t a(idx_t n)
{
    /* Two previous numerators:
       p_{-2} = 0, p_{-1} = 1   (the standard continued‑fraction recurrence) */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_curr;

    /* Cycle of multipliers for k ≥ 1: 1,1,1,46 */
    static const seq_t mult[4] = {1, 1, 1, 46};

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0)
            ak = 23;                     /* a_0 for √560 */
        else
            ak = mult[(k - 1) & 3];       /* period 4 after the first term */

        /* p_k = a_k * p_{k-1} + p_{k-2} */
        p_curr = ak * p_prev1 + p_prev2;

        /* shift values for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_curr;   /* p_n is the required numerator */
}
