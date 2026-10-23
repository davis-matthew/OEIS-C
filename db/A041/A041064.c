/*
 * A041064  Numerators of continued fraction convergents to sqrt(39).
 * https://oeis.org/A041064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=6; for n≥1 let c(n)=4 if n is odd, else 12;
   Then a(n)=c(n)*a(n-1)+a(n-2).  This computes the numerators of the
   convergents to sqrt(39). */

seq_t a(idx_t n)
{
    seq_t a_prev2 = 0;   /* p_{-1} */
    seq_t a_prev1 = 1;   /* p_0  */
    seq_t a_curr;

    for (idx_t k = 0; k <= n; ++k) {
        /* Continued‑fraction coefficient a_k for sqrt(39) */
        seq_t coeff;
        if (k == 0) {
            coeff = 6;
        } else {
            /* After the first term, the period is (4,12) */
            coeff = ((k - 1) % 2 == 0) ? 4 : 12;
        }

        a_curr = coeff * a_prev1 + a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
